#include "../collect/Collection.h"

//1.11改用Windows API，存入%localappdata%/Floating Ocean/Tetris/内

void checkMainFolder() {
    char dirName[100];
    sprintf(dirName, "%s\\Floating Ocean", getenv("localappdata"));
    DWORD fileAttribute = GetFileAttributesA(dirName);
    if (fileAttribute == INVALID_FILE_ATTRIBUTES || !(fileAttribute & FILE_ATTRIBUTE_DIRECTORY))
        CreateDirectory((PCSTR) dirName, NULL);
}

/**
 * 检查存储文件夹是否存在，不存在则创建
 */
void checkFolder() {
    checkMainFolder();
    char dirName[100];
    sprintf(dirName, "%s\\Floating Ocean\\Tetris", getenv("localappdata"));
    DWORD fileAttribute = GetFileAttributesA(dirName);
    if (fileAttribute == INVALID_FILE_ATTRIBUTES || !(fileAttribute & FILE_ATTRIBUTE_DIRECTORY))
        CreateDirectory((PCSTR) dirName, NULL);
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
 * @param key key=
 * @return 数据
 */
int queryDB(char *container, char *key) {
    checkFolder();
    return GetPrivateProfileInt(container, key, 0, getFileName());
}

/**
 * 写入数据
 * @param container [container]
 * @param key key=
 * @param data 内容
 */
void insertDB(char *container, char *key, int data) {
    checkFolder();
    char toSave[64];
    sprintf(toSave, "%d", data);
    WritePrivateProfileString(container, key, toSave, getFileName());
}