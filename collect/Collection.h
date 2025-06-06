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
#ifndef TETRIS_COLLECTION_H
#define TETRIS_COLLECTION_H

#endif //TETRIS_COLLECTION_H

#include "Structs.h"
#include "Colors.h"

//Game.c 变量
extern GameMode MODE_EZ, MODE_HD, MODE_IN;

extern GameMode currentGameMode;

//main.c 变量
extern const char *versionName;
extern const char *versionNameFull;

extern const int DIRECTION_LEFT, DIRECTION_RIGHT, DIRECTION_DOWN;

extern int N;
extern Fall nowFalling;
extern Block blocks[7];
extern Point currentMap[24][24];
extern ValidLine validLine[24];
extern Timer timer;
extern int nextBlock, nextRotate, savedBlock, savedRotate;
extern int score, darkLevel, trialMove, removedLines;
extern int challengeModeFault;
extern int appearAt;
extern double speedMultiply;
extern bool enablePreview, hidePreviewTemporarily, challengeModeEnabled, beyondEnabled, challengeComplete;
extern bool forceEndGame, mirrorEnabled;
extern DWORD speedMultiplyEnabledTime;
extern char **inputArgument;

//主页面 main.c
extern bool checkEnvironment(int argumentCount);

extern void showStartPage();

extern void showWelcomePage();

//方块 game/Block.c
extern void setInData(int d[7][4][4][4]);

extern void randomizeBlockColor();

extern void initializeBlock();

extern void initializeSavedBlock();

extern void initializeMap();

extern bool checkBlock(Fall what);

extern bool checkCanMove(int direction);

extern void putBlock(int rotate, int value);

extern void writeBlock(int rotate, int value);

extern void insertBlock(int rotate);

extern void clearBlock(int rotate);

extern void showBlock(int rotate);

extern void hideBlock(int rotate);

extern bool moveBlock(int direction, bool *ended);

extern bool rotateBlock(bool *ended);

extern void extractNextBlock(bool showNext);

extern bool appearBlock();

extern void updateValidBlocks();

extern void judgeLines();

extern void showSavedBlock();

extern void saveBlock();

//惩罚 game.Punishment.c
extern bool shuffleBlocks();

extern void sinkElevation();

extern void minusScore(int n);

extern void insertBarrier();

extern void mirrorPartially();

extern void mirrorTotally();

extern bool eraseBlockRandomly();

extern bool exchangeRowRandomly();

extern void changeSpeedRandomly();

extern void checkDarkenLevel(bool *ended);

extern void animateDarkenCover(int down, int up, int color);

extern void animateDarkenRecover(int level, int color);

extern void recoverDarkenLevel(bool fail);

extern void downDrawDarkenLevel();

extern void showRandomActionHint(const char *action);

//分数 game/Score.c
extern void printScore(int showScore);

extern void initializeScore();

//控制台 util/Console.c
extern void hideCursor();

extern void moveCursor(int x, int y);

extern void prepareTextPlacing(int x, int y, int color);

extern void placeText(const char *text, int x, int y, int color);

extern bool initConsoleFont();

extern void initConsoleColor();

extern void disableConsoleFeatures();

extern void centralizeWindow();

extern void resizeWindow(int x, int y);

extern void lockConsoleWindow();

extern void clearConsole();

extern void changeSubtitle(const char *subtitle);

extern bool initConsole();

//文件操作 util/File.c
extern void checkFolder();

extern const char *getFileName();

extern int queryDB(const char *container, const char *key);

extern void insertDB(const char *container, const char *key, int data);

//游戏 interface/Game.c
extern void endGame(bool force);

extern void showGreetings();

extern void startGame();

//属性 interface/Property.c
extern void changeDifficultySection(int index, int textColor, int borderColor, const char **text);

extern void showCurrentMode(const char **text, GameMode mode, int color);

extern void recoverTitle();

extern void noticeChallengeEnable();

extern bool showSelectView();

extern bool showBeyondSelectView();

extern int calculateLevel();

extern void refreshPreview();

extern void hidePreview(bool hide);

//基本函数 util/StdMethod.c
extern void swap(void *a, void *b, const size_t typeSize);

extern int randBetween(int x, int y);

extern void shuffleIntArray(int *array, int n);

extern void getCurrentGreeting(char *greet);

extern int hexToDec(char hex);

extern COLORREF hexToRGB(const char *hex);

//计时器 util/Timer.c
extern void *timeThread(void *args);

extern bool whenBreakTimer(); //abstract -> Game.c

//字体 util/Font.c
extern bool checkFont();

//主题控制 utilThemeCtrl.c
extern bool importCustomTheme(const char *file);