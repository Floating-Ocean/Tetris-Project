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

//三个难度
GameMode MODE_EZ = {"EZ", 0x00, 500, 1},
        MODE_HD = {"HD", 0x01, 300, 2},
        MODE_IN = {"IN", 0x02, 200, 5};
GameMode currentGameMode;

/**
 * 向控制台输出一个难度的选中状态
 * @param index 难度下标
 * @param textColor 更改文本颜色
 * @param borderColor 更改边框颜色
 * @param text 显示内容
 */
void printOne(int index, int textColor, int borderColor, char **text) {
    AwaitSettingTextInPosition(5, 5 + index * 8, borderColor);
    for (int i = 0; i < 36; i++) printf("■");
    for (int i = 0; i < 4; i++) {
        MoveCursor(5, 6 + index * 8 + i);
        for (int j = 0; j < 39; j++) printf("  ");
    }
    MoveCursor(5, 10 + index * 8);
    for (int i = 0; i < 36; i++) printf("■");
    SetTextInPosition(text[0], 8, 7 + index * 8, textColor);
    SetTextInPosition(text[1], 8, 8 + index * 8, textColor);
}

/**
 * 当前选中的游戏模式
 * @param text 显示内容
 * @param mode 游戏模式
 * @param color 更改颜色
 */
void showCurrentMode(char **text, GameMode mode, int color) {
    currentGameMode = mode;
    if (!beyondEnabled) printOne(mode.mode, color, color, text);
    AwaitSettingTextInPosition(5, 2, COLOR_MAIN_TEXT);
    printf("%s%s%s", "当前选中游戏模式：  ",
           beyondEnabled ? "BYD" : (mode.mode == 0x00 ? "EZ" : mode.mode == 0x01 ? "HD" : "IN"),
           " Mode   按空格键继续.");
}

/**
 * 未选中，恢复原标题
 */
void recoverTitle() {
    SetTextInPosition("选择一个游戏模式并按下对应按键以继续...         ", 5, 2, COLOR_MAIN_TEXT);
}

/**
 * 通知玩家触发挑战模式，并介绍游戏规则
 */
void noticeChallengeEnable() {
    system("cls & mode con cols=90 lines=20");
    PlaceWindowCentral();
    refreshTitleState("Challenge Mode Notification");
    SetTextInPosition(" — Challenge Mode Enabled — ", 29, 4, COLOR_FAULT);
    SetTextInPosition("本模式下所有难度惩罚次数", 24, 10, COLOR_MAIN_TEXT);
    SetTextInPosition("3次", 48, 10, COLOR_WARN);
    SetTextInPosition("判定", 51, 10, COLOR_MAIN_TEXT);
    SetTextInPosition("游戏失败", 55, 10, COLOR_FAULT);
    SetTextInPosition("按 任 意 键 继 续", 36, 14, COLOR_MAIN_TEXT);
    system("pause > nul");
    challengeModeEnabled = true;
    showSelectView();
}

/**
 * 显示选择页并处理交互
 */
bool showSelectView() {
    system("cls & mode con cols=82 lines=29");
    PlaceWindowCentral();
    refreshTitleState(challengeModeEnabled ? "Mode Choosing    Challenge Mode" : "Mode Choosing");
    SetTextInPosition("选择一个游戏模式并按下对应按键以继续...         ", 5, 2, COLOR_MAIN_TEXT);
    char *ez[2] = {"E   Easy Mode", "    半秒一次下落，长时间未消行将随机删除8个有效行内的几个格子."};
    char *hd[2] = {"H   Hard Mode", "    1/3秒一次下落，长时间未消行将随机删除16个有消行内的多个格子."};
    char *in[2] = {"I   Insane Mode", challengeModeEnabled ? "    1/5秒一次下落，长时间未消行将触发随机惩罚，游戏可以暂停."
                                                           : "    1/5秒一次下落，长时间未消行将触发随机惩罚，游戏无法暂停."};
    printOne(0, COLOR_SUB_TEXT, COLOR_MILD, ez);
    printOne(1, COLOR_SUB_TEXT, COLOR_MILD, hd);
    printOne(2, COLOR_SUB_TEXT, COLOR_MILD, in);
    bool selected = false, confirm = false;
    int challengeInput[] = {99, 104, 97, 108, 108, 101, 110, 103, 101, 13}, currentChallengeInputIndex = -1;
    int beyondInput[] = {72, 72, 80, 80, 75, 75, 77, 77,
                         80, 80, 80, 80, 80, 80, 80, 80, 80, 80}, currentBeyondInputIndex = -1;
    bool awaitDirectionInput = false;
    while (!confirm) { //未决定难度，等待输入
        if (kbhit()) {
            int input = getch();
            if (input == 224) {
                awaitDirectionInput = true; //方向键前置符
                continue;
            }
            if (awaitDirectionInput) {
                awaitDirectionInput = false;
                if (beyondInput[++currentBeyondInputIndex] == input) {
                    if (currentBeyondInputIndex == 17) {
                        return showBeyondSelectView();
                    }
                } else currentBeyondInputIndex = -1;
                //做一个方向键循环选择
                if (input == 72 || input == 75) { //key up & left.
                    if (currentGameMode.mode == MODE_EZ.mode) input = 105;
                    else if (currentGameMode.mode == MODE_HD.mode) input = 101;
                    else if (currentGameMode.mode == MODE_IN.mode) input = 104;
                } else if (input == 80 || input == 77) { //key down & right.
                    if (currentGameMode.mode == MODE_EZ.mode) input = 104;
                    else if (currentGameMode.mode == MODE_HD.mode) input = 105;
                    else if (currentGameMode.mode == MODE_IN.mode) input = 101;
                }
            } else {
                if (input == 27) {
                    showWelcomePage();
                    return false;
                }
                if (!challengeModeEnabled) { //读取输入challenge回车
                    if (challengeInput[++currentChallengeInputIndex] == input ||
                        (currentChallengeInputIndex < 9 && challengeInput[currentChallengeInputIndex] - 32 == input)) {
                        if (currentChallengeInputIndex == 9) {
                            noticeChallengeEnable();
                            break;
                        }
                    } else currentChallengeInputIndex = -1;
                }
            }
            switch (input) {
                case 69:
                case 101: //E
                    if (selected) {
                        if (currentGameMode.mode == MODE_EZ.mode) {
                            printOne(0, COLOR_SUB_TEXT, COLOR_MILD, ez);
                            selected = false;
                            recoverTitle();
                        } else {
                            printOne(1, COLOR_SUB_TEXT, COLOR_MILD, hd);
                            printOne(2, COLOR_SUB_TEXT, COLOR_MILD, in);
                            showCurrentMode(ez, MODE_EZ, COLOR_PASS);
                        }
                    } else {
                        selected = true;
                        showCurrentMode(ez, MODE_EZ, COLOR_PASS);
                    }
                    break;
                case 72:
                case 104: //H
                    if (selected) {
                        if (currentGameMode.mode == MODE_HD.mode) {
                            printOne(1, COLOR_SUB_TEXT, COLOR_MILD, hd);
                            selected = false;
                            recoverTitle();
                        } else {
                            printOne(0, COLOR_SUB_TEXT, COLOR_MILD, ez);
                            printOne(2, COLOR_SUB_TEXT, COLOR_MILD, in);
                            showCurrentMode(hd, MODE_HD, COLOR_WARN);
                        }
                    } else {
                        selected = true;
                        showCurrentMode(hd, MODE_HD, COLOR_WARN);
                    }
                    break;
                case 73:
                case 105: //I
                    if (selected) {
                        if (currentGameMode.mode == MODE_IN.mode) {
                            printOne(2, COLOR_SUB_TEXT, COLOR_MILD, in);
                            selected = false;
                            recoverTitle();
                        } else {
                            printOne(0, COLOR_SUB_TEXT, COLOR_MILD, ez);
                            printOne(1, COLOR_SUB_TEXT, COLOR_MILD, hd);
                            showCurrentMode(in, MODE_IN, COLOR_FAULT);
                        }
                    } else {
                        selected = true;
                        showCurrentMode(in, MODE_IN, COLOR_FAULT);
                    }
                    break;
                case 32: //决定!
                case 13: //回车也可以确定啦
                    confirm = selected;
                    break;
            }
        }
    }
    return true;
}

bool showBeyondSelectView() {
    system("cls & mode con cols=82 lines=37");
    PlaceWindowCentral();
    refreshTitleState("Inner Mode Choosing    Challenge Mode");
    SetTextInPosition("选择一个游戏模式并按下对应按键以继续...         ", 5, 2, COLOR_MAIN_TEXT);
    char *ez[2] = {"E   Easy Mode", "    半秒一次下落，长时间未消行将随机删除最后三行内的几个格子."};
    char *hd[2] = {"H   Hard Mode", "    1/3秒一次下落，长时间未消行将随机删除最后十行内的多个格子."};
    char *in[2] = {"I   Insane Mode", "    1/5秒一次下落，长时间未消行将触发随机惩罚，游戏可以暂停."};
    char *byd[2] = {"B   Beyond Mode", "    在Insane Mode的基础上，消行后进度条只恢复较少的值."};
    printOne(0, COLOR_SUB_TEXT, COLOR_MILD, ez);
    printOne(1, COLOR_SUB_TEXT, COLOR_MILD, hd);
    printOne(2, COLOR_SUB_TEXT, COLOR_MILD, in);
    printOne(3, COLOR_SUB_TEXT, COLOR_MILD, byd);
    bool selected = false, confirm = false, awaitDirectionInput = false;
    while (!confirm) { //未决定难度，等待输入
        if (kbhit()) {
            int input = getch();
            if (input == 224) {
                awaitDirectionInput = true; //方向键前置符
                continue;
            }
            if (awaitDirectionInput) {
                awaitDirectionInput = false;
                //做一个方向键循环选择
                if (input == 72 || input == 75) { //key up & left.
                    if (currentGameMode.mode == MODE_EZ.mode) input = 98;
                    else if (currentGameMode.mode == MODE_HD.mode) input = 101;
                    else if (beyondEnabled) input = 105;
                    else if (currentGameMode.mode == MODE_IN.mode) input = 104;
                } else if (input == 80 || input == 77) { //key down & right.
                    if (currentGameMode.mode == MODE_EZ.mode) input = 104;
                    else if (currentGameMode.mode == MODE_HD.mode) input = 105;
                    else if (beyondEnabled) input = 101;
                    else if (currentGameMode.mode == MODE_IN.mode) input = 98;
                }
            } else {
                if (input == 27) {
                    showWelcomePage();
                    return false;
                }
            }
            switch (input) {
                case 69:
                case 101: //E
                    if (selected) {
                        if (currentGameMode.mode == MODE_EZ.mode) {
                            printOne(0, COLOR_SUB_TEXT, COLOR_MILD, ez);
                            printOne(3, COLOR_SUB_TEXT, COLOR_MILD, byd);
                            selected = false;
                            beyondEnabled = false;
                            recoverTitle();
                        } else {
                            beyondEnabled = false;
                            printOne(1, COLOR_SUB_TEXT, COLOR_MILD, hd);
                            printOne(2, COLOR_SUB_TEXT, COLOR_MILD, in);
                            printOne(3, COLOR_SUB_TEXT, COLOR_MILD, byd);
                            showCurrentMode(ez, MODE_EZ, COLOR_PASS);
                        }
                    } else {
                        selected = true;
                        beyondEnabled = false;
                        showCurrentMode(ez, MODE_EZ, COLOR_PASS);
                        printOne(3, COLOR_SUB_TEXT, COLOR_MILD, byd);
                    }
                    break;
                case 72:
                case 104: //H
                    if (selected) {
                        if (currentGameMode.mode == MODE_HD.mode) {
                            printOne(1, COLOR_SUB_TEXT, COLOR_MILD, hd);
                            printOne(3, COLOR_SUB_TEXT, COLOR_MILD, byd);
                            selected = false;
                            beyondEnabled = false;
                            recoverTitle();
                        } else {
                            beyondEnabled = false;
                            printOne(0, COLOR_SUB_TEXT, COLOR_MILD, ez);
                            printOne(2, COLOR_SUB_TEXT, COLOR_MILD, in);
                            printOne(3, COLOR_SUB_TEXT, COLOR_MILD, byd);
                            showCurrentMode(hd, MODE_HD, COLOR_WARN);
                        }
                    } else {
                        selected = true;
                        beyondEnabled = false;
                        showCurrentMode(hd, MODE_HD, COLOR_WARN);
                        printOne(3, COLOR_SUB_TEXT, COLOR_MILD, byd);
                    }
                    break;
                case 73:
                case 105: //I
                    if (selected) {
                        if (!beyondEnabled && currentGameMode.mode == MODE_IN.mode) {
                            printOne(2, COLOR_SUB_TEXT, COLOR_MILD, in);
                            printOne(3, COLOR_SUB_TEXT, COLOR_MILD, byd);
                            selected = false;
                            beyondEnabled = false;
                            recoverTitle();
                        } else {
                            beyondEnabled = false;
                            printOne(0, COLOR_SUB_TEXT, COLOR_MILD, ez);
                            printOne(1, COLOR_SUB_TEXT, COLOR_MILD, hd);
                            printOne(3, COLOR_SUB_TEXT, COLOR_MILD, byd);
                            showCurrentMode(in, MODE_IN, COLOR_FAULT);
                        }
                    } else {
                        selected = true;
                        beyondEnabled = false;
                        showCurrentMode(in, MODE_IN, COLOR_FAULT);
                        printOne(3, COLOR_SUB_TEXT, COLOR_MILD, byd);
                    }
                    break;
                case 66:
                case 98: //B
                    if (selected) {
                        if (beyondEnabled) {
                            printOne(3, COLOR_SUB_TEXT, COLOR_MILD, byd);
                            selected = false;
                            beyondEnabled = false;
                            recoverTitle();
                        } else {
                            beyondEnabled = true;
                            printOne(0, COLOR_SUB_TEXT, COLOR_MILD, ez);
                            printOne(1, COLOR_SUB_TEXT, COLOR_MILD, hd);
                            printOne(2, COLOR_SUB_TEXT, COLOR_MILD, in);
                            printOne(3, COLOR_FAULT, COLOR_FAULT, byd);
                            showCurrentMode(in, MODE_IN, COLOR_FAULT);
                        }
                    } else {
                        selected = true;
                        beyondEnabled = true;
                        printOne(3, COLOR_FAULT, COLOR_FAULT, byd);
                        showCurrentMode(in, MODE_IN, COLOR_FAULT);
                    }
                    break;
                case 32: //决定!
                case 13: //回车也可以确定啦
                    confirm = selected;
                    break;
            }
        }
    }
    return true;
}

/**
 * 计算评级Level
 * @return level
 */
int calculateLevel() {
    int root[3] = {1, 4, 7};
    double increase[3] = {0.16, 0.32, 0.64}, e = 2.718281828459;
    return root[currentGameMode.mode] + (int) floor(sqrt((double) score / (e * increase[currentGameMode.mode])));
}

/**
 * 刷新状态查看
 */
void refreshPreview() {
    AwaitSettingTextInPosition(4, 1, COLOR_SUB_TEXT);
    printf("                                                                               ");
    if (!enablePreview) return;
    AwaitSettingTextInPosition(4, 1, COLOR_SUB_TEXT);
    printf("Difficulty:  %s. %d    Move Per Second:  %.02f    Removed Lines:  %d",
           beyondEnabled ? "BYD" : currentGameMode.modeName,
           (int) ((mirrorEnabled ? 2.0f : 1.0f) * (beyondEnabled ? 1.5f : 1.0f) * (float) calculateLevel()),
           1000 / (double) currentGameMode.speed * (1 / speedMultiply), removedLines);
}

/**
 * 是否隐藏Preview
 * @param hide 是否隐藏
 */
void hidePreview(bool hide) {
    if (hide) {
        hidePreviewTemporarily = enablePreview;
        if (hidePreviewTemporarily) {
            enablePreview = false;
            refreshPreview();
        }
    } else {
        if (hidePreviewTemporarily) {
            enablePreview = true;
            refreshPreview();
            hidePreviewTemporarily = false;
        }
    }
}