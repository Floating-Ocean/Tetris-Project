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

/**
 * 隐藏光标 (需要移动光标填充信息)
 */
void hideCursor() {
    const CONSOLE_CURSOR_INFO cursor_info = {1, 0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

/**
 * 移动光标
 * @param x x 轴
 * @param y y 轴
 */
void moveCursor(const int x, const int y) {
    const COORD pos = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

/**
 * 移动光标并设置颜色，等待输出内容
 * @param x x 轴
 * @param y y 轴
 * @param color 颜色
 */
void prepareTextPlacing(const int x, const int y, const int color) {
    moveCursor(x, y);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0 << 4 | color + 3);
}

/**
 * 移动光标、设置颜色、输出内容
 * @param text 输出内容
 * @param x x 轴
 * @param y y 轴
 * @param color 颜色
 */
void placeText(const char *text, const int x, const int y, const int color) {
    prepareTextPlacing(x, y, color);
    printf("%s", text);
}

/**
 * 初始化控制台自定义字体
 */
bool initConsoleFont() {
    if (!checkFont()) {
        centralizeWindow();
        changeSubtitle("");
        printf(
            "\n  未在你的设备上找到字体：Sarasa Mono SC\n\n  你可以转到下面的网址下载该字体并重启应用。\n\n  https://github.com/Floating-Ocean/Tetris-Project\n\n  https://mirrors.tuna.tsinghua.edu.cn/github-release/be5invis/Sarasa-Gothic/Sarasa Gothic version 0.37.4/\n\n  谢谢.");
        system("pause > nul");
        return false;
    }
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof cfi;
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;
    cfi.dwFontSize.Y = 20; // 设置字体大小
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL; // 字体粗细 FW_BOLD
    wcscpy_s(cfi.FaceName, 32, L"Sarasa Mono SC"); // 设置字体
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
    return true;
}

/**
 * 初始化自定义颜色
 */
void initConsoleColor() { // NOLINT(*-no-recursion)
    CONSOLE_SCREEN_BUFFER_INFOEX cs_bi = {sizeof(CONSOLE_SCREEN_BUFFER_INFOEX)};
    GetConsoleScreenBufferInfoEx(GetStdHandle(STD_OUTPUT_HANDLE), &cs_bi);
    if (queryDB("TetrisSetting", "ThemeType") == 2) {
        COLORREF colors[16];
        for (int i = 0; i < 16; i++) {
            const char *tags[16] = {
                "BackgroundColor", "BackgroundColor", "BackgroundColor",
                "BlockColor_0", "BlockColor_1", "BlockColor_2", "BlockColor_3", "BlockColor_4",
                "BlockColor_5", "BlockColor_6", "ForegroundStrongColor", "ForegroundModestColor",
                "ForegroundMildColor", "FaultColor", "PassColor", "WarningColor"
            };
            char nowColor[10];
            GetPrivateProfileString("TetrisCustomTheme", tags[i], "NULL", nowColor, sizeof nowColor, getFileName());
            if (strcmp(nowColor, "NULL") == 0) {
                insertDB("TetrisSetting", "ThemeType", 0); // 主题损坏，回滚主题
                initConsoleColor();
                return;
            }
            colors[i] = hexToRGB(nowColor);
        }
        memcpy(cs_bi.ColorTable, colors, sizeof colors); // 把颜色配置拷进去
    } else {
        constexpr COLORREF lightColors[16] = {
            RGB(238, 238, 238), // #eeeeee 白色 0
            RGB(238, 238, 238), // #eeeeee 白色 1
            RGB(238, 238, 238), // #eeeeee 白色 2
            RGB(173, 20, 87),   // #ad1457 粉色 0
            RGB(106, 27, 154),  // #6a1b9a 紫色 1
            RGB(40, 53, 154),   // #28359a 靛青 2
            RGB(2, 119, 189),   // #0277bd 蓝色 3
            RGB(0, 105, 92),    // #00695c 青色 4
            RGB(158, 157, 36),  // #827717 柠檬 5
            RGB(239, 108, 0),   // #ef6c00 橙色 6
            RGB(12, 12, 12),    // #121212 深黑色 7
            RGB(66, 66, 66),    // #424242 灰色 8
            RGB(189, 189, 189), // #bdbdbd 浅灰色 9
            RGB(229, 57, 53),   // #e53935 红色 10
            RGB(67, 160, 71),   // #43a047 绿色 11
            RGB(255, 160, 0)    // #ffa000 琥珀色 12
        };
        constexpr COLORREF darkColors[16] = {
            RGB(12, 12, 12),    // #121212 黑色 0
            RGB(12, 12, 12),    // #121212 黑色 1
            RGB(12, 12, 12),    // #121212 黑色 2
            RGB(236, 64, 122),  // #ec407a 粉色 0
            RGB(171, 71, 188),  // #ab47bc 紫色 1
            RGB(121, 134, 203), // #7986cb 靛青 2
            RGB(41, 182, 246),  // #29b6f6 蓝色 3
            RGB(77, 182, 172),  // #4db6ac 青色 4
            RGB(212, 225, 87),  // #d4e157 柠檬 5
            RGB(255, 167, 38),  // #ffa726 橙色 6
            RGB(250, 250, 250), // #fafafa 亮白色 7
            RGB(189, 189, 189), // #bdbdbd 浅灰色 8
            RGB(66, 66, 66),    // #424242 深灰色 9
            RGB(239, 83, 80),   // #ef5350 红色 10
            RGB(76, 175, 80),   // #4caf50 绿色 11
            RGB(255, 202, 40)   // #ffca28 琥珀色 12
        };
        const COLORREF *currentColorSet = queryDB("TetrisSetting", "ThemeType") ? lightColors : darkColors;
        memcpy(cs_bi.ColorTable, currentColorSet, sizeof currentColorSet); // 拷贝颜色配置
    }
    SetConsoleScreenBufferInfoEx(GetStdHandle(STD_OUTPUT_HANDLE), &cs_bi); // 应用设置
}

/**
 * 移除一些无用功能
 */
void disableConsoleFeatures() {
    DWORD mode;
    GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);
    mode &= ~ENABLE_QUICK_EDIT_MODE;
    mode &= ~ENABLE_INSERT_MODE;
    mode &= ~ENABLE_MOUSE_INPUT;
    SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode);
}

/**
 * 使窗口居中
 */
void centralizeWindow() {
    const int screenX = GetSystemMetrics(SM_CXFULLSCREEN), screenY = GetSystemMetrics(SM_CYFULLSCREEN); // 屏幕去掉任务栏的长宽
    RECT windowRect;
    GetWindowRect(GetConsoleWindow(), &windowRect);
    const int windowX = windowRect.right - windowRect.left, windowY = windowRect.bottom - windowRect.top;
    MoveWindow(GetConsoleWindow(), (screenX - windowX) / 2, (screenY - windowY) / 3 * 2, windowX, windowY, TRUE);
}

/**
 * 强制修改控制台窗口大小
 * @param x 目标列数
 * @param y 目标行数
 */
void resizeWindow(const int x, const int y) {
    // 取消窗口最大化
    if (IsZoomed(GetConsoleWindow())) {
        ShowWindow(GetConsoleWindow(), SW_RESTORE);
        Sleep(100);
    }

    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo);

    // 保证缓冲区至少足够容纳当前窗口和目标窗口尺寸
    const SHORT maxX = max(x, bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1);
    const SHORT maxY = max(y, bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1);

    for (int i = 0; i < 3; i++) {
        const COORD maxBufferSize = {maxX, maxY};
        if (SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), maxBufferSize)) break;
        Sleep(50); // 短暂延迟后重试
    }

    // 设置窗口大小
    for (int i = 0; i < 3; i++) {
        const SMALL_RECT windowArea = {0, 0, (SHORT) (x - 1), (SHORT) (y - 1)};
        if (SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowArea)) break;
        Sleep(50);
    }

    // 设置目标缓冲区尺寸
    const COORD bufferSize = {(SHORT)x, (SHORT)y};
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bufferSize);

    // 触发窗口重绘
    SetWindowPos(GetConsoleWindow(), nullptr, 0, 0, 0, 0,
                 SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);

    lockConsoleWindow();
}


/**
 * 禁止窗口调整大小和全屏
 */
void lockConsoleWindow() {
    LONG style = GetWindowLong(GetConsoleWindow(), GWL_STYLE);
    style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX); // 移除可调整大小的边框和最大化按钮
    SetWindowLong(GetConsoleWindow(), GWL_STYLE, style);

    // 触发窗口重绘
    SetWindowPos(GetConsoleWindow(), nullptr, 0, 0, 0, 0,
                 SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

/**
 * 清空控制台
 */
void clearConsole() {
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo)) return;
    const DWORD consoleSize = bufferInfo.dwSize.X * bufferInfo.dwSize.Y;
    const COORD startPos = {0, 0};
    DWORD charsWritten;
    FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', consoleSize, startPos, &charsWritten); // 使用空格填充整个缓冲区
    prepareTextPlacing(0, 0, COLOR_MAIN_TEXT);
}

/**
 * 修改副标题
 * @param subtitle 副标题
 */
void changeSubtitle(const char *subtitle) {
    char curTitle[450];
    sprintf(curTitle, "title Tetris v%s - Floating Ocean    %s", versionName, subtitle);
    system(curTitle);
}

/**
 * 初始化控制台，包括字符集，标题等等
 */
bool initConsole() {
    prepareTextPlacing(0, 0, COLOR_MAIN_TEXT);
    SetConsoleOutputCP(65001);
    initConsoleColor();
    if (!initConsoleFont()) return false;
    disableConsoleFeatures();
    lockConsoleWindow();
    hideCursor();
    setvbuf(stdout, nullptr, _IONBF, 0);
    return true;
}
