/*
 * Copyright (C) 2022-2024 Floating Ocean
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
 * Timer线程
 * @param args
 * @return 是否有键盘事件
 */
void *timeThread(void *args) {
    if (timer.currentTimeMills - timer.previousTimeMills >= currentGameMode.speed * speedMultiply) {
        timer.previousTimeMills = GetTickCount();
        timer.currentTimeMills = timer.previousTimeMills;
    } else { //处理上一次剩下的时间
        const DWORD delta = timer.currentTimeMills - timer.previousTimeMills;
        timer.currentTimeMills = GetTickCount();
        timer.previousTimeMills = timer.currentTimeMills - delta;
    }
    while (true) {
        //beyond加速模式持续18秒，其余变速持续9秒
        if (speedMultiply != 1.0 && GetTickCount() - speedMultiplyEnabledTime >= (beyondEnabled && speedMultiply > 1 ? 18000 : 9000)) {
            speedMultiply = 1.0; //恢复Speed
            if (enablePreview) refreshPreview();
        }
        if (timer.currentTimeMills - timer.previousTimeMills >= currentGameMode.speed * speedMultiply) {
            pthread_exit("true"); //无键盘事件，可以下落
            break;
        }
        timer.currentTimeMills = GetTickCount();
        if (whenBreakTimer()) {
            pthread_exit("false"); //有键盘事件，打断
            break;
        }
    }
}