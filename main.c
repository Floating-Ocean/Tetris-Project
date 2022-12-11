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

#include "collect/Collection.h"

//-----版本控制-----
const char *versionName = "2.1.0";
const char *versionNameFull = "2.1.0.221210";

//-----常量区-----
const int DIRECTION_LEFT = -1, DIRECTION_RIGHT = 1, DIRECTION_DOWN = 0;

//-----全局变量区-----
int N = 24;
Fall nowFalling;
Block blocks[7];
Point currentMap[24][24];
ValidLine validLine[24];
Timer timer;
int nextBlock, nextRotate, savedBlock, savedRotate;
int score = 0, darkLevel = 0, trialMove = 0, removedLines = 0;
int challengeModeFault = 0;
double speedMultiply = 1.0;
bool enablePreview = false, hidePreviewTemporarily = false, challengeModeEnabled = false, beyondEnabled = false;
bool forceEndGame = false;
DWORD speedMultiplyEnabledTime;
char **inputArgument;

/**
 * 主函数
 * @return 0
 */
int main(int argc, char **argv) {
    inputArgument = argv;
    if(!checkEnvironment(argc)) return 0;
    if(!initializeConsole()) return 0;
    initializeBlock();
    showStartPage();
}

/**
 * 检查是否从conhost打开，处理拖拽进入的主题文件的导入
 * @param argumentCount 命令行参数数量
 * @return 是否继续游戏
 */
bool checkEnvironment(int argumentCount){
    //强行从conhost打开,Windows Terminal的可玩度太低了
    SetConsoleOutputCP(65001);
    if (argumentCount != 2 || strcmp(inputArgument[1], "/openConhost") != 0) {
        if(argumentCount == 2){
            if(importCustomTheme(inputArgument[1])) printf("\n\n\n  主题导入成功，可在下次打开游戏时切换.");
            system("pause > nul");
            return false;
        }
        system("title Tetris Loader");
        printf("\n  Please Wait . . .");
        if(!checkFont()){
            system("cls");
            printf("\n  未在你的设备上找到字体：Sarasa Mono SC\n\n  你可以转到下面的网址下载该字体并重启应用。\n\n  https://github.com/Floating-Ocean/Tetris-Project\n\n  https://mirrors.tuna.tsinghua.edu.cn/github-release/be5invis/Sarasa-Gothic/Sarasa Gothic version 0.37.4/\n\n  谢谢.");
            system("pause > nul");
            return false;
        }
        Sleep(500);
        char command[350] = "start conhost ";
        strcat(command, inputArgument[0]);
        strcat(command, " /openConhost");
        system(command);
        return false;
    }
    return true;
}

/**
 * 显示最初的开始页
 */
void showStartPage(){
    system("cls & mode con cols=50 lines=34");
    PlaceWindowCentral();
    refreshTitleState("");
    printf("\n\n\n\n           俄罗斯方块 - Tetris Project\n\n\n"
           "           古 典 游 戏     创 新 玩 法\n\n\n\n\n\n\n\n\n"
           "               点 按 任 意 键 继 续\n\n\n\n\n\n\n\n"
           "               Version %s\n\n\n\n"
           "          Copyright ©2022 Floating Ocean.\n"
           "                All Rights Reserved.", versionNameFull);
    while (true) //等待切换主题重开
        if (kbhit()) {
            int input = getch();
            if (input == 116) { //T键切换主题
                int themeRange = queryDB("TetrisSetting", "ImportedTheme") ? 2 : 1, oldTheme = queryDB("TetrisSetting", "ThemeType"),
                    newTheme = oldTheme + 1 > themeRange ? 0 : oldTheme + 1;
                insertDB("TetrisSetting", "ThemeType",  newTheme > themeRange ? themeRange : newTheme);
                char command[350] = "start conhost ";
                strcat(command, inputArgument[0]);
                strcat(command, " /openConhost");
                system(command);
                return;
            }else if(input == 27) return; //按esc退出游戏
            else break;
        }
    showWelcomePage();
}

/**
 * 显示欢迎页
 */
void showWelcomePage(){
    system("cls & mode con cols=75 lines=28");
    PlaceWindowCentral();
    refreshTitleState("Welcome");
    char greeting[50], userName[MAX_PATH];
    getCurrentGreeting(greeting);
    DWORD size = MAX_PATH;
    GetUserName(userName, &size);
    printf("\n\n    %s，%s.\n\n\n"
           "    欢迎来到  Tetris Project!\n\n\n"
           "    让我们一起了解一下这款游戏的使用吧!\n\n"
           "    1.使用 ← → 键左右移动方块，↑ 键顺时针旋转方块，↓ 键加速下落.\n"
           "    2.使用 R 键重新开始游戏，K 键暂停游戏，C 键存储方块，P键查看状态.\n"
           "    3.消除一行后，会得到一定的分数，分数取决于连消数和游戏难度.\n"
           "    4.在界面左右侧进度柱进度归零前未消行，将会按照游戏难度进行惩罚.\n"
           "    5.在传统俄罗斯方块游戏中能做到的，这里应该都可以做到.\n\n"
           "    更多游戏介绍请移步 floating-ocean.github.io/tetrisproj/ \n\n\n"
           "    点按键盘任意键继续.\n\n\n\n"
           "    ©2022 Floating Ocean.\n"
           "    All Rights Reserved.", greeting, userName);
    while (true) //等待按esc或继续游戏
        if (kbhit()) {
            int input = getch();
            if(input == 27) {
                showStartPage();
                return; //按esc返回开始页
            }else break;
        }
    system("cls");
    startGame();
}