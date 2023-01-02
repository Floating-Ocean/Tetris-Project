/*
 * Copyright (C) 2022-2023 Floating Ocean
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
 * 格式化score的输出(前置空格)
 * @param showScore 分数
 */
void printScore(int showScore) {
    if (showScore < 10) printf("      %d 分  ", showScore);
    else if (showScore < 100) printf("     %d 分   ", showScore);
    else printf("     %d 分  ", showScore);
}

/**
 * 初始化游戏分数系统&ui
 */
void initializeScore() {
    SetTextInPosition("    当前分数   ", 5, 5, COLOR_MAIN_TEXT);
    score = 0;
    AwaitSettingTextInPosition(5, 7, COLOR_SUB_TEXT);
    printScore(score);
    SetTextInPosition("    历史最高   ", 5, 11, COLOR_MAIN_TEXT);
    AwaitSettingTextInPosition(5, 13, COLOR_SUB_TEXT);
    printScore(challengeModeEnabled ? queryDB("TetrisData", "ChallengeBestRecord") :
               queryDB("TetrisData", "BestRecord"));
    bool tmp = beyondEnabled;
    beyondEnabled = false;
    animateDarkenRecover(N, COLOR_MAIN_TEXT); //刷新Darken值的ui显示
    beyondEnabled = tmp;
}