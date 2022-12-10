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

/**
 * 主函数
 * @return 0
 */
int main(int argc, char **argv) {
    //强行从conhost打开,Windows Terminal的可玩度太低了
    if (argc != 2 || strcmp(argv[1], "/openConhost") != 0 != 0) {
        system("title Tetris Loader");
        printf("Please Wait . . .");
        Sleep(500);
        char command[350] = "start conhost ";
        strcat(command, argv[0]);
        strcat(command, " /openConhost");
        system(command);
        system("exit");
        return 0;
    }
    initializeConsole();
    initializeBlock();
    system("mode con cols=50 lines=34");
    PlaceWindowCentral();
    refreshTitleState("");
    printf("\n\n\n\n           俄罗斯方块 - Tetris Project\n\n\n"
           "           古 典 游 戏     创 新 玩 法\n\n\n\n\n\n\n\n\n"
           "               点 按 任 意 键 继 续\n\n\n\n\n\n\n\n"
           "               Version %s\n\n\n\n"
           "          Copyright ©2022 Floating Ocean.\n"
           "                All Rights Reserved.", versionNameFull);
    system("pause > nul & cls & mode con cols=75 lines=30");
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
           "    Windows11以下系统需要安装 Cascadia Mono 字体.\n\n\n"
           "    点按键盘任意键继续.\n\n\n\n"
           "    ©2022 Floating Ocean.", greeting, userName);
    system("pause>nul & cls");
    startGame();
}