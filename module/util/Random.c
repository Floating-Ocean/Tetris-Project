/*
* Copyright (C) 2022-2025 Floating Ocean
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "../../collect/Collection.h"

// Adapted from testlib.h by Mike Mirzayanov
// ------ begin ------
unsigned long long seed = 3905348978240129619LL;
constexpr unsigned long long multiplier = 0x5DEECE66DLL;
constexpr unsigned long long addend = 0xBLL;
constexpr unsigned long long mask = (1LL << 48) - 1;

long long nextBits(const int bits) { // NOLINT(*-no-recursion)
    if (bits <= 48) {
        seed = (seed * multiplier + addend) & mask;
        return (long long) (seed >> (48 - bits));
    }
    if (bits > 63) exit(1);
    const long long left = nextBits(31) << 32, right = nextBits(32);
    return left ^ right;
}

int next(const int n) {
    if (n <= 0) exit(1);
    if ((n & -n) == n) return (int) ((n * nextBits(31)) >> 31);
    const long long limit = INT_MAX / n * n;
    long long bits;
    do {
        bits = nextBits(31);
    } while (bits >= limit);
    return (int) (bits % n);
}
// ------ end ------

/**
 * 在指定范围生成随机数
 * @param x 左区间
 * @param y 右区间
 * @return 随机数
 */
int randBetween(const int x, const int y) {
    return next(y - x + 1) + x;
}
