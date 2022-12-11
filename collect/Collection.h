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

#include "Structs.h"

#ifndef TETRIS_METHODS_H
#define TETRIS_METHODS_H

#endif //TETRIS_METHODS_H

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
extern double speedMultiply;
extern bool enablePreview, hidePreviewTemporarily, challengeModeEnabled, beyondEnabled;
extern bool forceEndGame;
extern DWORD speedMultiplyEnabledTime;
extern char **inputArgument;

//主页面 main.c
extern bool checkEnvironment(int argumentCount);

extern void showStartPage();

extern void showWelcomePage();

//方块 Block.c
extern void setInData(int d[7][4][4][4]);

extern void randomizeBlockColor();

extern void initializeBlock();

extern void printScore(int score);

extern void initializeScore();

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

extern bool moveBlock(int direction);

extern bool rotateBlock();

extern void extractNextBlock();

extern bool appearBlock();

extern void judgeLines();

extern void animateDarkenCover(int down, int up, int color);

extern void animateDarkenRecover(int level, int color);

extern void recoverDarkenLevel(bool fail);

extern void downDrawDarkenLevel();

extern void showRandomActionHint(char *action);

extern bool eraseBlockRandomly();

extern bool exchangeRowRandomly();

extern void changeSpeedRandomly();

extern void checkDarkenLevel();

extern void showSavedBlock();

extern void saveBlock();

//控制台 Console.c
extern void HideCursor();

extern void MoveCursor(int x, int y);

extern void AwaitSettingTextInPosition(int x, int y, int color);

extern void SetTextInPosition(char *text, int x, int y, int color);

extern bool initializeFont();

extern void initializeColor();

extern void DisableFeatures();

extern void PlaceWindowCentral();

extern void refreshTitleState(char *state);

extern bool initializeConsole();

//文件操作 File.c
extern void checkFolder();

extern const char *getFileName();

extern int queryDB(char *container, char *key);

extern void insertDB(char *container, char *key, int data);

//游戏 Game.c
extern void endGame(bool force);

extern void showGreetings();

extern void startGame();

//属性 Property.c
extern void printOne(int index, int textColor, int borderColor, char **text);

extern void showCurrentMode(char **text, GameMode mode, int color);

extern void recoverTitle();

extern void noticeChallengeEnable();

extern bool showSelectView();

extern bool showBeyondSelectView();

extern int calculateLevel();

extern void refreshPreview();

extern void hidePreview(bool hide);

//基本函数 StdMethod.c
extern int randBetween(int x, int y);

extern void shuffleIntArray(int *array, int n);

extern void getCurrentGreeting(char *greet);

//计时器 Timer.c
extern void *timeThread(void *args);

extern bool whenBreakTimer(); //abstract -> Game.c

//字体 Font.c
extern bool checkFont();