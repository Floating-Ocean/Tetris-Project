#include "../collect/Collection.h"

/**
 * 隐藏光标 (需要移动光标填充信息)
 */
void HideCursor() {
    CONSOLE_CURSOR_INFO cursor_info = {1, 0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

/**
 * 移动光标
 * @param x x轴
 * @param y y轴
 */
void MoveCursor(int x, int y) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = (SHORT) x;
    pos.Y = (SHORT) y;
    SetConsoleCursorPosition(handle, pos);
}

/**
 * 移动光标并设置颜色，等待输出内容
 * @param x x轴
 * @param y y轴
 * @param color 颜色
 */
void AwaitSettingTextInPosition(int x, int y, int color) {
    MoveCursor(x, y);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0 << 4 | color + 3);
}

/**
 * 移动光标、设置颜色、输出内容
 * @param text 输出内容
 * @param x x轴
 * @param y y轴
 * @param color 颜色
 */
void SetTextInPosition(char *text, int x, int y, int color) {
    AwaitSettingTextInPosition(x, y, color);
    printf("%s", text);
}

/**
 * 初始化控制台自定义字体
 */
void initializeFont() {
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof cfi;
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;
    cfi.dwFontSize.Y = 20;  //设置字体大小
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL; //字体粗细 FW_BOLD
    wcscpy_s(cfi.FaceName, 32, L"Cascadia Mono");  //设置字体，必须是控制台已有的，安装了Windows Terminal的设备自带字体Cascadia Mono
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

/**
 * 初始化自定义颜色
 */
void initializeColor() {
    COLORREF lightColors[16] = {
            RGB(238, 238, 238), //#ffeeeeee 白色 0
            RGB(238, 238, 238), //#ffeeeeee 白色 1
            RGB(238, 238, 238), //#ffeeeeee 白色 2
            RGB(173, 20, 87), //#ffad1457 粉色 3
            RGB(106, 27, 154), //#ff6a1b9a 紫色 4
            RGB(40, 53, 154), //#ff28359a 靛青 5
            RGB(2, 119, 189), //#ff0277bd 蓝色 6
            RGB(0, 105, 92), //#ff00695c青色 7
            RGB(158, 157, 36), //#ff827717 柠檬 8
            RGB(239, 108, 0), //#ffef6c00 橙色 9
            RGB(12, 12, 12), //#ff121212 深黑色 10
            RGB(66, 66, 66), //#ff424242 灰色 11
            RGB(189, 189, 189), //#ffbdbdbd 浅灰色 12
            RGB(229, 57, 53), //#ffe53935 红色 13
            RGB(67, 160, 71), //#ff43a047 绿色 14
            RGB(255, 160, 0) //#ffffa000 琥珀色 15
    }, darkColors[16] = {
            RGB(12, 12, 12), //#ff121212 黑色 0
            RGB(12, 12, 12), //#ff121212 黑色 1
            RGB(12, 12, 12), //#ff121212 黑色 2
            RGB(236, 64, 122), //#ffec407a 粉色 3
            RGB(171, 71, 188), //#ffab47bc 紫色 4
            RGB(121, 134, 203), //#ff7986cb 靛青 5
            RGB(41, 182, 246), //#ff29b6f6 蓝色 6
            RGB(77, 182, 172), //#ff4db6ac青色 7
            RGB(212, 225, 87), //#ffd4e157 柠檬 8
            RGB(255, 167, 38), //#ffffa726 橙色 9
            RGB(250, 250, 250), //#fffafafa 亮白色 10
            RGB(189, 189, 189), //#ffbdbdbd 浅灰色 11
            RGB(66, 66, 66), //#ff424242 深灰色 12
            RGB(239, 83, 80), //#ffef5350 红色 13
            RGB(76, 175, 80), //#ff4caf50 绿色 14
            RGB(255, 202, 40) //#ffffca28 琥珀色 15
    };
    CONSOLE_SCREEN_BUFFER_INFOEX csbi = {sizeof(CONSOLE_SCREEN_BUFFER_INFOEX)};
    GetConsoleScreenBufferInfoEx(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    memcpy(csbi.ColorTable, queryDB("TetrisSetting", "LightTheme") ? lightColors : darkColors, queryDB("TetrisSetting", "LightTheme") ? sizeof lightColors : sizeof darkColors); //把颜色配置拷进去
    SetConsoleScreenBufferInfoEx(GetStdHandle(STD_OUTPUT_HANDLE), &csbi); //应用设置
}

/**
 * 移除一些无用功能
 */
void DisableFeatures() {
    HANDLE stdInHandle = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(stdInHandle, &mode);
    mode &= ~ENABLE_QUICK_EDIT_MODE;
    mode &= ~ENABLE_INSERT_MODE;
    mode &= ~ENABLE_MOUSE_INPUT;
    SetConsoleMode(stdInHandle, mode);
}

/**
 * 使窗口居中
 */
void PlaceWindowCentral() {
    int screenX = GetSystemMetrics(SM_CXFULLSCREEN), screenY = GetSystemMetrics(SM_CYFULLSCREEN); //屏幕去掉任务栏的长宽
    RECT windowRect;
    GetWindowRect(GetConsoleWindow(), &windowRect);
    int windowX = windowRect.right - windowRect.left, windowY = windowRect.bottom - windowRect.top;
    MoveWindow(GetConsoleWindow(), (screenX - windowX) / 2, (screenY - windowY) / 3 * 2, windowX, windowY, TRUE);
}

/**
 * 刷新标题状态
 * @param state 当前状态
 */
void refreshTitleState(char *state) {
    char curTitle[450];
    sprintf(curTitle, "title Tetris v%s - Floating Ocean    %s", versionName, state);
    system(curTitle);
}

/**
 * 初始化控制台，包括字符集，标题等等
 */
void initializeConsole() {
    system("color 0A");
    SetConsoleOutputCP(65001);
    DisableFeatures();
    initializeColor();
    initializeFont();
    HideCursor();
}