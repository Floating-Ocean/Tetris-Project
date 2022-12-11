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

#include "../collect/Collection.h"

/**
 * 导入自定义主题
 * @param file 文件路径
 */
bool importCustomTheme(char *file) {
    //step1. 检查文件后缀
    int startIndex;
    for(int i = 0; file[i] != '\0'; i ++)if(file[i] == '\\') startIndex = i;
    for(int i = startIndex; file[i] != '\0'; i++) if(file[i] == '.') startIndex = i;
    if(strcmp(&file[startIndex + 1], "fotp") != 0) {
        printf("\n  主题文件格式错误.");
        return false;
    }
    //step2.读文件
    char *tags[14] = {"BackgroundColor", "ForegroundStrongColor",
                      "ForegroundModestColor", "ForegroundMildColor", "PassColor", "WarningColor", "FaultColor",
                      "BlockColor_0", "BlockColor_1", "BlockColor_2", "BlockColor_3", "BlockColor_4", "BlockColor_5", "BlockColor_6"};
    char fetched[14][12];
    for(int i=0;i<14;i++){
        GetPrivateProfileString("CustomTheme", tags[i], "NULL", fetched[i], sizeof fetched[i], file);
        if(strcmp(fetched[i], "NULL") == 0 || !(strlen(fetched[i]) == 7 || strlen(fetched[i]) == 8)) {
            printf("\n  主题文件损坏.");
            return false;
        }
    }
    insertDB("TetrisSetting", "ImportedTheme", true);
    for(int i=0;i<14;i++) WritePrivateProfileString("TetrisCustomTheme", tags[i], fetched[i], getFileName());
    return true;
}
