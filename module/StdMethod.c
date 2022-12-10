/**
   Copyright 2022 Floating Ocean

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */
#include "../collect/Collection.h"

/**
 * 在指定范围生成随机数
 * @param x 左区间
 * @param y 右区间
 * @return 随机数
 */
int randBetween(int x, int y) {
    return rand() % (y - x + 1) + x;
}

/**
 * 随机化并不强的int数组随机化
 * @param array int数组
 * @param n 数组大小
 */
void shuffleIntArray(int *array, int n) {
    for (int i = 0; i < n; i++) {
        int ind = randBetween(0, n - 1), cur = array[i];
        array[i] = array[ind];
        array[ind] = cur;
    }
}

/**
 * 根据时间来获取问候语
 * @param greet 存储问候语的地址
 */
void getCurrentGreeting(char *greet){
    SYSTEMTIME systemTime;
    GetLocalTime(&systemTime);
    int hour = systemTime.wHour;
    if(hour >= 6 && hour <= 11) strcpy(greet, "早上好");
    else if(hour == 12) strcpy(greet, "中午好");
    else if(hour >= 13 && hour <= 16) strcpy(greet, "下午好");
    else if(hour >= 17 && hour <= 23) strcpy(greet, "晚上好");
    else strcpy(greet, "早点睡");
}