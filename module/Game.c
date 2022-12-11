/*
 * Copyright (C) 2022 Floating Ocean
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

#include "../collect/Collection.h"

/**
 * 结束游戏
 * @param force 是否是按"R"键终止了游戏
 */
void endGame(bool force) {
    refreshTitleState(challengeModeEnabled ? "Game End    Challenge Mode" : "Game End");
    SetTextInPosition(force ? "  游戏被终止  " : (challengeModeEnabled ? (challengeModeFault >= 3 && removedLines <= 25 ? "  挑战失败.  " : "  挑战结束！  ") : "  你寄了嘞！  "), 80, 5, 7);
    if(!force && challengeModeFault >= 3 && removedLines <= 25) animateDarkenCover(N, 1, 10); //做一个变成黄条的动画
    else animateDarkenCover(N, darkLevel + 1, force ? 12 : 10); //做一个变成黄条的动画
    for (int i = 0; i < 4; i++) {
        MoveCursor(84, 7 + i);
        for (int j = 0; j < 4; j++) {
            printf("  ");
        }
    }
    SetTextInPosition("  按空格重开 ", 80, 9, 7);
    if(challengeModeFault >= 3 && removedLines <= 25){ //挑战失败不计入成绩
        SetTextInPosition("             ", 5, 11, 7);
        SetTextInPosition("   分数不计入 ", 5, 12, 7);
        SetTextInPosition("           ", 5, 13, 7);
    }else{
        if (queryDB("TetrisData", "ChallengeBestRecord") < score) {
            insertDB("TetrisData", "ChallengeBestRecord", score);
            SetTextInPosition("             ", 5, 11, 7);
            SetTextInPosition("    NEW BEST   ", 5, 12, 7);
            SetTextInPosition("           ", 5, 13, 7);
        }
    }
    if (queryDB("TetrisData", "BestRecord") < score) {
        insertDB("TetrisData", "BestRecord", score);
        SetTextInPosition("             ", 5, 11, 7);
        SetTextInPosition("    NEW BEST   ", 5, 12, 7);
        SetTextInPosition("           ", 5, 13, 7);
    }
    challengeModeEnabled = false;
    beyondEnabled = false;
    while (true) //等待按空格重开
        if (kbhit()) {
            int input = getch();
            if (input == 32 || input == 13) { //空格或回车
                system("cls");
                break;
            }
        }
    startGame();
}

/**
 * 初始化并输出Greetings到对应ui区
 */
void showGreetings() {
    char *greetings01[34] = {"我就知道,", "这里啊,", "消行消行~", "哎哎哎， ", "总会有种", "长按上键", "万物，尘埃.",
                             "EZ难度呢,", "HD难度啊,", "IN难度呀,", "翻 (低头)", "有没有可能", "有没有可能", "哎呀我丢", "你干嘛~",
                             "两只老虎", "这里是", "5YW9LStfLg==", "Challenge", "↑↑↓↓", "新的事物，", "超越一切，",
                             "你说这C语言", "哼 哼 哼", "你是一个一个", "存点吧孩子", "这里是，", "Challenge~", "挑战未知，",
                             "哪里更新游戏", "作者啊作者", "游戏这玩意儿", "你...搁这儿", "俄罗斯方块"};
    char *greetings02[34] = {"你会看这里.", "什么都没有!", "GKD! 好兄弟.", "要寄了！！", "被坑的感觉.", "长条变风车.",
                             "尘埃落定吗?", "它不叫摁着.", "它不叫高清.", "它不叫里面.", "给你看啥呢?", "这是一句话.", "你在看这里.",
                             "咋还有空格.", "嘿嘿嘿呦", "爱跳舞~", "Greetings!", ".+-..---.-__", "Type it.", "←←→→", "在未知处...",
                             "创死凡人！", "咋这么难搞", "啊啊啊啊啊", "俄罗斯方块啊", "不存白不存", "这里是那里", "25 Kill！", "方可超越未知",
                             "GitHub？", "格子间距真宽", "一瞬间就崩力", "叠罗汉呢??", "旋转四分钟~"};
    int index = randBetween(0, 33);
    SetTextInPosition(greetings01[index], 8, 20, 7);
    SetTextInPosition(greetings02[index], 8, 22, 7);
    SetTextInPosition("From Ocean", 8, 25, 8);
}

/**
 * 开始游戏
 */
void startGame() {
    if(!showSelectView()) return; //先显示难度选择页
    system("cls & mode con cols=100 lines=30");
    PlaceWindowCentral();
    char _curTitle[35];
    sprintf(_curTitle, "In Game    %s Mode    %s", beyondEnabled ? "BYD" : currentGameMode.modeName, challengeModeEnabled ? "Challenge Mode" : "");
    refreshTitleState(_curTitle);
    srand(time(NULL)); //初始化随机数种子
    darkLevel = 0;
    trialMove = 0;
    savedBlock = -1;
    savedRotate = -1;
    speedMultiply = 1.0;
    challengeModeFault = 0;
    removedLines = 0;
    initializeMap();
    showGreetings();
    extractNextBlock();
    forceEndGame = false;
    if (queryDB("TetrisSetting", "EnablePreview")) {
        enablePreview = true;
        refreshPreview();
    }
    while (true) {
        judgeLines();
        if (!appearBlock()) { //不能塞下下一个方块就是寄了
            endGame(false);
            break;
        }
        extractNextBlock();
        while (true) { //使用线程实现可靠的Timer
            pthread_t thread;
            pthread_create(&thread, NULL, timeThread, NULL);
            void *result;
            pthread_join(thread, &result); //是否有键盘事件
            if (strcmp((char *) result, "false") == 0) { //处理键盘事件
                int input = getch();
                if (input == 72) rotateBlock(); //key up
                else if (input == 75) moveBlock(DIRECTION_LEFT); //key left
                else if (input == 77) moveBlock(DIRECTION_RIGHT); //key right
                else if (input == 80) { //key down
                    if (!moveBlock(DIRECTION_DOWN)) break;
                } else if (input == 114 || input == 27) { //输入R or esc: 重开
                    forceEndGame = true;
                    endGame(true);
                    break;
                } else if (input == 107) { //输入K: 暂停
                    if (currentGameMode.mode == MODE_IN.mode) continue;
                    int turn = 0;
                    bool showingWhat = false;
                    char curTitle[35];
                    sprintf(curTitle, "In Game    %s Mode    Paused", beyondEnabled ? "BYD" : currentGameMode.modeName); //标题显示暂停
                    refreshTitleState(curTitle);
                    animateDarkenCover(N, darkLevel + 1, 12); //做一个变成黄条的动画
                    while (true) {
                        if (kbhit() && getch() != 107) break;
                        if (turn % 100 == 0) {
                            showingWhat = !showingWhat;
                            turn = 1;
                            SetTextInPosition(showingWhat ? "  游戏已暂停  " : "  任意键继续  ", 80, 5, blocks[nextBlock].color);
                        }
                        Sleep(10);
                        turn++;
                    }
                    animateDarkenCover(N, darkLevel + 1, 7);
                    SetTextInPosition("  下一个方块  ", 80, 5, 7);
                    char oldTitle[35];
                    sprintf(oldTitle, "In Game    %s Mode", beyondEnabled ? "BYD" : currentGameMode.modeName); //恢复标题
                    refreshTitleState(oldTitle);
                } else if (input == 99) { //输入C: 存方块
                    saveBlock();
                } else if (input == 112) {
                    if (!hidePreviewTemporarily) { //防止冲突
                        enablePreview = !enablePreview;
                        insertDB("TetrisSetting", "EnablePreview", enablePreview);
                        refreshPreview();
                    }
                }
            } else {//下落
                if (!moveBlock(DIRECTION_DOWN)) break;
            }
        }
        if (forceEndGame) break;
    }
    if (!forceEndGame) startGame();
}

/**
 * 有键盘事件时打破Timer
 * @return 是否打破
 * @see Timer#void* timeThread(void* args)
 */
bool whenBreakTimer() {
    return kbhit();
}