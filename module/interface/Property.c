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

// 三个难度
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
void changeDifficultySection(const int index, const int textColor, const int borderColor, const char **text) {
    prepareTextPlacing(5, 5 + index * 8, borderColor);
    for (int i = 0; i < 36; i++) printf("■");
    for (int i = 0; i < 4; i++) {
        moveCursor(5, 6 + index * 8 + i);
        for (int j = 0; j < 39; j++) printf("  ");
    }
    moveCursor(5, 10 + index * 8);
    for (int i = 0; i < 36; i++) printf("■");
    placeText(text[0], 8, 7 + index * 8, textColor);
    placeText(text[1], 8, 8 + index * 8, textColor);
}

/**
 * 当前选中的游戏模式
 * @param text 显示内容
 * @param mode 游戏模式
 * @param color 更改颜色
 */
void showCurrentMode(const char **text, const GameMode mode, const int color) {
    currentGameMode = mode;
    if (!beyondEnabled) changeDifficultySection(mode.mode, color, color, text);
    prepareTextPlacing(5, 2, COLOR_MAIN_TEXT);
    printf("%s%s%s", "当前选中游戏模式：  ",
           beyondEnabled ? "BYD" : mode.mode == 0x00 ? "EZ" : mode.mode == 0x01 ? "HD" : "IN",
           " Mode   按空格键继续.");
}

/**
 * 未选中，恢复原标题
 */
void recoverTitle() {
    placeText("选择一个游戏模式并按下对应按键以继续...         ", 5, 2, COLOR_MAIN_TEXT);
}

/**
 * 通知玩家触发挑战模式，并介绍游戏规则
 */
void noticeChallengeEnable() { // NOLINT(*-no-recursion)
    clearConsole();
    resizeWindow(90, 20);
    centralizeWindow();
    changeSubtitle("Challenge Mode Notification");
    placeText(" — Challenge Mode Enabled — ", 29, 4, COLOR_FAULT);
    placeText("本模式下所有难度惩罚次数", 24, 10, COLOR_MAIN_TEXT);
    placeText("3次", 48, 10, COLOR_WARN);
    placeText("判定", 51, 10, COLOR_MAIN_TEXT);
    placeText("游戏失败", 55, 10, COLOR_FAULT);
    placeText("按 任 意 键 继 续", 36, 14, COLOR_MAIN_TEXT);
    system("pause > nul");
    challengeModeEnabled = true;
    showSelectView();
}

/**
 * 显示选择页并处理交互
 */
bool showSelectView() { // NOLINT(*-no-recursion)
    clearConsole();
    resizeWindow(82, 29);
    centralizeWindow();
    changeSubtitle(challengeModeEnabled ? "Mode Choosing    Challenge Mode" : "Mode Choosing");
    placeText("选择一个游戏模式并按下对应按键以继续...         ", 5, 2, COLOR_MAIN_TEXT);
    const char *ez[2] = {"E   Easy Mode", "    半秒一次下落，长时间未消行将随机删除几个有效行内的几个格子."};
    const char *hd[2] = {"H   Hard Mode", "    1/3秒一次下落，长时间未消行将触发随机惩罚，大概率消除格子."};
    const char *in[2] = {
        "I   Insane Mode", challengeModeEnabled
                               ? "    1/5秒一次下落，长时间未消行将触发随机惩罚，游戏可以暂停."
                               : "    1/5秒一次下落，长时间未消行将触发随机惩罚，游戏无法暂停."
    };
    changeDifficultySection(0, COLOR_SUB_TEXT, COLOR_MILD, ez);
    changeDifficultySection(1, COLOR_SUB_TEXT, COLOR_MILD, hd);
    changeDifficultySection(2, COLOR_SUB_TEXT, COLOR_MILD, in);
    bool selected = false, confirm = false;
    int currentChallengeInputIndex = -1, currentBeyondInputIndex = -1;
    bool awaitDirectionInput = false;
    while (!confirm) {
        // 未决定难度，等待输入
        if (kbhit()) {
            int input = getch();
            if (input == 224) {
                awaitDirectionInput = true; // 方向键前置符
                continue;
            }
            if (awaitDirectionInput) {
                constexpr int beyondInput[] = {72, 72, 80, 80, 75, 75, 77, 77, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80};
                awaitDirectionInput = false;
                if (beyondInput[++currentBeyondInputIndex] == input) {
                    if (currentBeyondInputIndex == 17) {
                        return showBeyondSelectView();
                    }
                } else currentBeyondInputIndex = -1;
                // 做一个方向键循环选择
                if (!selected && (input == 72 || input == 75 || input == 80 || input == 77)) {
                    input = 101; // 没有选中按方向键选中第一难度
                } else if (input == 72 || input == 75) {
                    // key up & left
                    if (currentGameMode.mode == MODE_EZ.mode) input = 105;
                    else if (currentGameMode.mode == MODE_HD.mode) input = 101;
                    else if (currentGameMode.mode == MODE_IN.mode) input = 104;
                } else if (input == 80 || input == 77) {
                    // key down & right
                    if (currentGameMode.mode == MODE_EZ.mode) input = 104;
                    else if (currentGameMode.mode == MODE_HD.mode) input = 105;
                    else if (currentGameMode.mode == MODE_IN.mode) input = 101;
                }
            } else {
                if (input == 27) {
                    showWelcomePage();
                    return false;
                }
                if (!challengeModeEnabled) {
                    constexpr int challengeInput[] = {99, 104, 97, 108, 108, 101, 110, 103, 101, 13};
                    // 读取输入 challenge + 回车
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
                case 101: // E
                    if (selected) {
                        if (currentGameMode.mode == MODE_EZ.mode) {
                            changeDifficultySection(0, COLOR_SUB_TEXT, COLOR_MILD, ez);
                            selected = false;
                            recoverTitle();
                        } else {
                            changeDifficultySection(1, COLOR_SUB_TEXT, COLOR_MILD, hd);
                            changeDifficultySection(2, COLOR_SUB_TEXT, COLOR_MILD, in);
                            showCurrentMode(ez, MODE_EZ, COLOR_PASS);
                        }
                    } else {
                        selected = true;
                        showCurrentMode(ez, MODE_EZ, COLOR_PASS);
                    }
                    break;
                case 72:
                case 104: // H
                    if (selected) {
                        if (currentGameMode.mode == MODE_HD.mode) {
                            changeDifficultySection(1, COLOR_SUB_TEXT, COLOR_MILD, hd);
                            selected = false;
                            recoverTitle();
                        } else {
                            changeDifficultySection(0, COLOR_SUB_TEXT, COLOR_MILD, ez);
                            changeDifficultySection(2, COLOR_SUB_TEXT, COLOR_MILD, in);
                            showCurrentMode(hd, MODE_HD, COLOR_WARN);
                        }
                    } else {
                        selected = true;
                        showCurrentMode(hd, MODE_HD, COLOR_WARN);
                    }
                    break;
                case 73:
                case 105: // I
                    if (selected) {
                        if (currentGameMode.mode == MODE_IN.mode) {
                            changeDifficultySection(2, COLOR_SUB_TEXT, COLOR_MILD, in);
                            selected = false;
                            recoverTitle();
                        } else {
                            changeDifficultySection(0, COLOR_SUB_TEXT, COLOR_MILD, ez);
                            changeDifficultySection(1, COLOR_SUB_TEXT, COLOR_MILD, hd);
                            showCurrentMode(in, MODE_IN, COLOR_FAULT);
                        }
                    } else {
                        selected = true;
                        showCurrentMode(in, MODE_IN, COLOR_FAULT);
                    }
                    break;
                case 32: // 决定!
                case 13: // 回车也可以确定啦
                    confirm = selected;
                    break;
                default: ;
            }
        }
    }
    return true;
}

bool showBeyondSelectView() {
    clearConsole();
    resizeWindow(82, 37);
    centralizeWindow();
    changeSubtitle("Inner Mode Choosing    Challenge Mode");
    placeText("选择一个游戏模式并按下对应按键以继续...         ", 5, 2, COLOR_MAIN_TEXT);
    const char *ez[2] = {"E   Easy Mode", "    半秒一次下落，长时间未消行将随机删除几个有效行内的几个格子."};
    const char *hd[2] = {"H   Hard Mode", "    1/3秒一次下落，长时间未消行将触发随机惩罚，大概率消除格子."};
    const char *in[2] = {"I   Insane Mode", "    1/5秒一次下落，长时间未消行将触发随机惩罚，游戏可以暂停."};
    const char *byd[2] = {"B   Beyond Mode", "    在Insane Mode的基础上，消行后进度条只恢复较少的值."};
    changeDifficultySection(0, COLOR_SUB_TEXT, COLOR_MILD, ez);
    changeDifficultySection(1, COLOR_SUB_TEXT, COLOR_MILD, hd);
    changeDifficultySection(2, COLOR_SUB_TEXT, COLOR_MILD, in);
    changeDifficultySection(3, COLOR_SUB_TEXT, COLOR_MILD, byd);
    bool selected = false, confirm = false, awaitDirectionInput = false;
    while (!confirm) {
        // 未决定难度，等待输入
        if (kbhit()) {
            int input = getch();
            if (input == 224) {
                awaitDirectionInput = true; // 方向键前置符
                continue;
            }
            if (awaitDirectionInput) {
                awaitDirectionInput = false;
                // 做一个方向键循环选择
                if (!selected && (input == 72 || input == 75 || input == 80 || input == 77)) {
                    input = 101; // 没有选中按方向键选中第一难度
                } else if (input == 72 || input == 75) {
                    // key up & left.
                    if (currentGameMode.mode == MODE_EZ.mode) input = 98;
                    else if (currentGameMode.mode == MODE_HD.mode) input = 101;
                    else if (beyondEnabled) input = 105;
                    else if (currentGameMode.mode == MODE_IN.mode) input = 104;
                } else if (input == 80 || input == 77) {
                    // key down & right.
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
                case 101: // E
                    if (selected) {
                        if (currentGameMode.mode == MODE_EZ.mode) {
                            changeDifficultySection(0, COLOR_SUB_TEXT, COLOR_MILD, ez);
                            changeDifficultySection(3, COLOR_SUB_TEXT, COLOR_MILD, byd);
                            selected = false;
                            beyondEnabled = false;
                            recoverTitle();
                        } else {
                            beyondEnabled = false;
                            changeDifficultySection(1, COLOR_SUB_TEXT, COLOR_MILD, hd);
                            changeDifficultySection(2, COLOR_SUB_TEXT, COLOR_MILD, in);
                            changeDifficultySection(3, COLOR_SUB_TEXT, COLOR_MILD, byd);
                            showCurrentMode(ez, MODE_EZ, COLOR_PASS);
                        }
                    } else {
                        selected = true;
                        beyondEnabled = false;
                        showCurrentMode(ez, MODE_EZ, COLOR_PASS);
                        changeDifficultySection(3, COLOR_SUB_TEXT, COLOR_MILD, byd);
                    }
                    break;
                case 72:
                case 104: // H
                    if (selected) {
                        if (currentGameMode.mode == MODE_HD.mode) {
                            changeDifficultySection(1, COLOR_SUB_TEXT, COLOR_MILD, hd);
                            changeDifficultySection(3, COLOR_SUB_TEXT, COLOR_MILD, byd);
                            selected = false;
                            beyondEnabled = false;
                            recoverTitle();
                        } else {
                            beyondEnabled = false;
                            changeDifficultySection(0, COLOR_SUB_TEXT, COLOR_MILD, ez);
                            changeDifficultySection(2, COLOR_SUB_TEXT, COLOR_MILD, in);
                            changeDifficultySection(3, COLOR_SUB_TEXT, COLOR_MILD, byd);
                            showCurrentMode(hd, MODE_HD, COLOR_WARN);
                        }
                    } else {
                        selected = true;
                        beyondEnabled = false;
                        showCurrentMode(hd, MODE_HD, COLOR_WARN);
                        changeDifficultySection(3, COLOR_SUB_TEXT, COLOR_MILD, byd);
                    }
                    break;
                case 73:
                case 105: // I
                    if (selected) {
                        if (!beyondEnabled && currentGameMode.mode == MODE_IN.mode) {
                            changeDifficultySection(2, COLOR_SUB_TEXT, COLOR_MILD, in);
                            changeDifficultySection(3, COLOR_SUB_TEXT, COLOR_MILD, byd);
                            selected = false;
                            beyondEnabled = false;
                            recoverTitle();
                        } else {
                            beyondEnabled = false;
                            changeDifficultySection(0, COLOR_SUB_TEXT, COLOR_MILD, ez);
                            changeDifficultySection(1, COLOR_SUB_TEXT, COLOR_MILD, hd);
                            changeDifficultySection(3, COLOR_SUB_TEXT, COLOR_MILD, byd);
                            showCurrentMode(in, MODE_IN, COLOR_FAULT);
                        }
                    } else {
                        selected = true;
                        beyondEnabled = false;
                        showCurrentMode(in, MODE_IN, COLOR_FAULT);
                        changeDifficultySection(3, COLOR_SUB_TEXT, COLOR_MILD, byd);
                    }
                    break;
                case 66:
                case 98: // B
                    if (selected) {
                        if (beyondEnabled) {
                            changeDifficultySection(3, COLOR_SUB_TEXT, COLOR_MILD, byd);
                            selected = false;
                            beyondEnabled = false;
                            recoverTitle();
                        } else {
                            beyondEnabled = true;
                            changeDifficultySection(0, COLOR_SUB_TEXT, COLOR_MILD, ez);
                            changeDifficultySection(1, COLOR_SUB_TEXT, COLOR_MILD, hd);
                            changeDifficultySection(2, COLOR_SUB_TEXT, COLOR_MILD, in);
                            changeDifficultySection(3, COLOR_FAULT, COLOR_FAULT, byd);
                            showCurrentMode(in, MODE_IN, COLOR_FAULT);
                        }
                    } else {
                        selected = true;
                        beyondEnabled = true;
                        changeDifficultySection(3, COLOR_FAULT, COLOR_FAULT, byd);
                        showCurrentMode(in, MODE_IN, COLOR_FAULT);
                    }
                    break;
                case 32: // 决定!
                case 13: // 回车也可以确定啦
                    confirm = selected;
                    break;
                default: ;
            }
        }
    }
    return true;
}

/**
 * 计算评级 Level
 * @return level
 */
int calculateLevel() {
    constexpr int root[3] = {1, 4, 7};
    constexpr double increase[3] = {0.16, 0.32, 0.64};
    constexpr double e = 2.718281828459;
    return root[currentGameMode.mode] + (int) floor(sqrt((double) score / (e * increase[currentGameMode.mode])));
}

/**
 * 刷新状态查看
 */
void refreshPreview() {
    prepareTextPlacing(4, 1, COLOR_SUB_TEXT);
    printf("                                                                               ");
    if (!enablePreview) return;
    prepareTextPlacing(4, 1, COLOR_SUB_TEXT);
    printf("Difficulty:  %s. %d    Move Per Second:  %.02f    Removed Lines:  %d",
           beyondEnabled ? "BYD" : currentGameMode.modeName,
           (int) ((mirrorEnabled ? 2.0f : 1.0f) * (beyondEnabled ? 1.5f : 1.0f) * (float) calculateLevel()),
           1000 / (double) currentGameMode.speed * (1 / speedMultiply), removedLines);
}

/**
 * 是否隐藏Preview
 * @param hide 是否隐藏
 */
void hidePreview(const bool hide) {
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