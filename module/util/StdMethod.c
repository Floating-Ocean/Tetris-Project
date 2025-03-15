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


/**
 * C语言的 "泛型" 交换函数
 * @param a A
 * @param b B
 * @param typeSize 类型的大小 sizeof
 */
void swap(void *a, void *b, const size_t typeSize) {
    unsigned char tmp[typeSize]; // 无动态内存分配，防止内存泄漏
    memcpy(tmp, a, typeSize);
    memcpy(a, b, typeSize);
    memcpy(b, tmp, typeSize);
}

/**
 * 随机化并不强的 int 数组随机化
 * @param array int 数组
 * @param n 数组大小
 */
void shuffleIntArray(int *array, const int n) {
    for (int i = 0; i < n; i++) {
        const int ind = randBetween(0, n - 1), cur = array[i];
        array[i] = array[ind];
        array[ind] = cur;
    }
}

/**
 * 根据时间来获取问候语
 * @param greet 存储问候语的地址
 */
void getCurrentGreeting(char *greet) {
    const char *greets[5][5] = {
        "早上好呀", "Hey, 早上好", "呼呼~ 早上了呢", "又是新的一天呢", "早安",
        "中午好", "午安", "12点了", "吃了没", "午安",
        "下午哩", "下午好呐", "又到下午力", "下午好哇", "午安",
        "晚上好呀", "又到晚上咯", "又在晚上看到你力", "一天过得好快鸭", "晚安",
        "不早了", "早点睡呀", "呼呼~好困呐", "晚安捏", "晚安"
    };
    SYSTEMTIME systemTime;
    GetLocalTime(&systemTime);
    const int hour = systemTime.wHour;
    int index = 4;
    if (hour >= 6 && hour <= 11) index = 0;
    else if (hour == 12) index = 1;
    else if (hour >= 13 && hour <= 16) index = 2;
    else if (hour >= 17 && hour <= 23) index = 3;
    strcpy(greet, greets[index][randBetween(0, 4)]);
}

/**
 * 一位十六进制转十进制
 * @param hex
 * @return
 */
int hexToDec(const char hex) {
    return hex >= '0' && hex <= '9'
               ? hex - '0'
               : hex >= 'A' && hex <= 'Z'
                     ? hex - 'A' + 10
                     : hex - 'a' + 10;
}

/**
 * 将 hex 颜色转换为 rgb 表达式
 * @param hex 形如 0xffffff 和 #ffffff 的颜色
 * @return rgb 色值
 */
COLORREF hexToRGB(const char *hex) {
    const int size = strlen(hex) == 7 ? 7 : 8; //支持 0xffffff 和 #ffffff
    return RGB(hexToDec(hex[size - 6]) * 16 + hexToDec(hex[size - 5]),
               hexToDec(hex[size - 4]) * 16 + hexToDec(hex[size - 3]),
               hexToDec(hex[size - 2]) * 16 + hexToDec(hex[size - 1]));
}
