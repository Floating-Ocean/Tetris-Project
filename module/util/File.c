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

// 1.11 改用 Windows API，存入 "%localappdata%/Floating Ocean/Tetris/" 内

/**
 * 检查存储文件夹的父文件夹是否存在，不存在则创建
 */
void checkMainFolder() {
    char dirName[100];
    sprintf(dirName, "%s\\Floating Ocean", getenv("localappdata"));
    const DWORD fileAttribute = GetFileAttributesA(dirName);
    if (fileAttribute == INVALID_FILE_ATTRIBUTES || !(fileAttribute & FILE_ATTRIBUTE_DIRECTORY))
        CreateDirectory(dirName, nullptr);
}

/**
 * 检查存储文件夹是否存在，不存在则创建
 */
void checkFolder() {
    checkMainFolder();
    char dirName[100];
    sprintf(dirName, "%s\\Floating Ocean\\Tetris", getenv("localappdata"));
    const DWORD fileAttribute = GetFileAttributesA(dirName);
    if (fileAttribute == INVALID_FILE_ATTRIBUTES || !(fileAttribute & FILE_ATTRIBUTE_DIRECTORY))
        CreateDirectory(dirName, nullptr);
}

/**
 * 返回文件绝对路径
 * @return 文件绝对路径
 */
const char *getFileName() {
    static char fileName[100];
    sprintf(fileName, "%s\\Floating Ocean\\Tetris\\shared_prefs.fot", getenv("localappdata"));
    return fileName;
}

/**
 * 读取数据
 * @param container [container]
 * @param key key =
 * @return 数据
 */
int queryDB(const char *container, const char *key) {
    checkFolder();
    return GetPrivateProfileInt(container, key, 0, getFileName());
}

/**
 * 写入数据
 * @param container [container]
 * @param key key =
 * @param data 内容
 */
void insertDB(const char *container, const char *key, const int data) {
    checkFolder();
    char toSave[64];
    sprintf(toSave, "%d", data);
    WritePrivateProfileString(container, key, toSave, getFileName());
}