/**
   Copyright 2022 Floating Ocean

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */
#include "../collect/Collection.h"

/**
 * 导入方块数据
 * @param d 方块数据
 */
void setInData(int d[7][4][4][4]) {
    for (int p = 0; p < 7; p++)
        for (int k = 0; k < 4; k++)
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    blocks[p].data[k][i][j] = d[p][k][i][j];
}

/**
 * 随机化方块颜色
 */
void randomizeBlockColor() {
    int colors[7];
    for (int i = 0; i < 7; i++) colors[i] = i;
    shuffleIntArray(colors, 7);
    for (int i = 0; i < 7; i++) blocks[i].color = colors[i];
}

/**
 * 初始化方块数据
 */
void initializeBlock() {
    int d[7][4][4][4] = {
            {//T字形
                    {{0, 0, 0, 0}, {1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
                    {{0, 0, 1, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
                    {{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 1}, {0, 0, 0, 0}},
                    {{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}}
            },
            {//L字形
                    {{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}},
                    {{0, 0, 0, 0}, {1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}},
                    {{0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
                    {{0, 0, 0, 0}, {0, 0, 0, 1}, {0, 1, 1, 1}, {0, 0, 0, 0}}
            },
            {//反L字形
                    {{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}},
                    {{0, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}},
                    {{0, 1, 1, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
                    {{0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 0, 1}, {0, 0, 0, 0}}
            },
            {//Z字形
                    {{0, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
                    {{0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
                    {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 0}},
                    {{0, 0, 0, 0}, {0, 0, 1, 0}, {0, 1, 1, 0}, {0, 1, 0, 0}}
            },
            {//反Z字形
                    {{0, 0, 0, 0}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
                    {{0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}},
                    {{0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}, {0, 0, 0, 0}},
                    {{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 0}}
            },
            {//正方形
                    {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
                    {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
                    {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}},
                    {{0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}}
            },
            {//长方形
                    {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},
                    {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
                    {{0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 0}},
                    {{0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}}
            }
    };
    setInData(d); //导入数据
}

/**
 * 格式化score的输出
 * @param score 分数
 */
void printScore(int score) {
    if (score < 10) printf("      %d 分  ", score);
    else if (score < 100) printf("     %d 分   ", score);
    else printf("     %d 分  ", score);
}

/**
 * 初始化游戏分数系统&ui
 */
void initializeScore() {
    SetTextInPosition("    当前分数   ", 5, 5, 7);
    score = 0;
    AwaitSettingTextInPosition(5, 7, 8);
    printScore(score);
    SetTextInPosition("    历史最高   ", 5, 11, 7);
    AwaitSettingTextInPosition(5, 13, 8);
    printScore(challengeModeEnabled ? queryDB("TetrisData", "ChallengeBestRecord") : queryDB("TetrisData", "BestRecord"));
    bool tmp = beyondEnabled;
    beyondEnabled = false;
    animateDarkenRecover(N, 7); //刷新Darken值的ui显示
    beyondEnabled = tmp;
}

/**
 * 初始化已保存方块ui
 */
void initializeSavedBlock() {
    SetTextInPosition("  方块未存入  ", 80, 18, 7);
    SetTextInPosition("  按C键存取,  ", 80, 22, 8);
    SetTextInPosition("  备不时之需  ", 80, 24, 8);
}

/**
 * 初始化主交互ui
 */
void initializeMap() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0 << 4 | 9 + 3);
    printf("\n\n    ");
    for (int i = 0; i < 9; i++) printf("■"); //left
    printf("  ");
    for (int i = 0; i < N + 2; i++) printf("■"); //mid
    printf("  ");
    for (int i = 0; i < 9; i++) printf("■"); //right
    printf("\n    ");
    for (int i = 0; i < N; i++) {
        printf(i == 14 ? "  " : "■");
        for (int j = 0; j < 7; j++) printf(i == 13 || i == 15 ? "■" : "  ");
        printf(i == 14 ? "    ■" : "■  ■");
        for (int j = 0; j < N; j++) {
            currentMap[i][j].color = 7;
            currentMap[i][j].state = 0;
            printf("  ");
        }
        printf(i == 11 ? "■" : "■  ■");
        for (int j = 0; j < 7; j++) printf(i == 10 || i == 12 ? "■" : "  ");
        printf(i == 11 ? "   \n    " : "■\n    ");
    }
    for (int i = 0; i < 9; i++) printf("■");
    printf("  ");
    for (int i = 0; i < N + 2; i++) printf("■");
    printf("  ");
    for (int i = 0; i < 9; i++) printf("■");
    printf("\n  ");
    initializeScore(); //初始化分数ui
    initializeSavedBlock(); //初始化保存方块ui
    randomizeBlockColor(); //随机化方块颜色
}

/**
 * 检查当前方块所在位置是否合法
 * @param what 当前方块
 * @return 是否合法
 */
bool checkBlock(Fall what) {
    Block now = blocks[what.index];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (now.data[what.rotate][i][j] == 1 &&
                (currentMap[what.point[0] + i][what.point[1] + j].state == 1 || //当前位置有方块
                 what.point[0] + i < 0 || what.point[0] + i >= N || //当前位置左边界超过范围
                 what.point[1] + j < 0 || what.point[1] + j >= N)) //右边界
                return false;
        }
    }
    return true;
}

/**
 * 检查方块是否可以向指定方向移动
 * @param direction 方向
 * @return 是否可移动
 */
bool checkCanMove(int direction) {
    Fall what = {nowFalling.index, nowFalling.rotate, nowFalling.point[0], nowFalling.point[1]}; //clone
    if (direction == DIRECTION_DOWN) what.point[0]++;
    else what.point[1] += direction; //direction: -1, 1
    clearBlock(what.rotate); //暂时清除原方块占位来检测是否可移动
    bool result = checkBlock(what);
    insertBlock(what.rotate); //复位
    return result;
}

/**
 * 填充方块入状态数组
 * @param rotate 旋转方向
 * @param value 方块状态
 */
void putBlock(int rotate, int value) {
    Block now = blocks[nowFalling.index];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            if (now.data[rotate][i][j] == 1) {
                currentMap[nowFalling.point[0] + i][nowFalling.point[1] + j].color = value ? now.color : 7; //方块颜色
                currentMap[nowFalling.point[0] + i][nowFalling.point[1] + j].state = value;
            }
        }
}

/**
 * 输出方块至ui
 * @param rotate 旋转方向
 * @param value 方块状态
 */
void writeBlock(int rotate, int value) {
    Block now = blocks[nowFalling.index];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            if (now.data[rotate][i][j] == 1) {
                AwaitSettingTextInPosition(26 + 2 * (nowFalling.point[1] + j), 3 + nowFalling.point[0] + i, now.color);
                printf(value ? "■" : "  ");
            }
        }
}

/**
 * 填充方块入状态数组：状态1
 * @param rotate 旋转方向
 * @see void putBlock(int rotate, int value);
 */
void insertBlock(int rotate) {
    putBlock(rotate, 1);
}

/**
 * 填充方块入状态数组：状态0
 * @param rotate 旋转方向
 * @see void putBlock(int rotate, int value);
 */
void clearBlock(int rotate) {
    putBlock(rotate, 0);
}

/**
 * 输出方块至ui：状态1
 * @param rotate 旋转方向
 * @see void writeBlock(int rotate, int value);
 */
void showBlock(int rotate) {
    writeBlock(rotate, 1);
}

/**
 * 输出方块至ui：状态0
 * @param rotate 旋转方向
 * @see void writeBlock(int rotate, int value);
 */
void hideBlock(int rotate) {
    writeBlock(rotate, 0);
}

/**
 * 尝试向指定方向移动方块
 * @param direction 方向
 * @return 是否移动成功
 */
bool moveBlock(int direction) {
    if (!checkCanMove(direction)) return false; //检查是否可移动
    hideBlock(nowFalling.rotate);
    clearBlock(nowFalling.rotate); //清空方块
    if (direction == DIRECTION_DOWN) nowFalling.point[0]++;
    else nowFalling.point[1] += direction;
    insertBlock(nowFalling.rotate);
    showBlock(nowFalling.rotate); //输出移动后的状态
    checkDarkenLevel(); //检查Darken值
    return true;
}

/**
 * 尝试顺时针旋转方块
 * @return 是否旋转成功
 */
bool rotateBlock() {
    clearBlock(nowFalling.rotate);
    int preRotate = nowFalling.rotate;
    nowFalling.rotate = (nowFalling.rotate + 1) % 4; //4次旋转回到起始
    bool result = checkBlock(nowFalling);
    if (result) { //可以旋转
        hideBlock(preRotate);
        insertBlock(nowFalling.rotate);
        showBlock(nowFalling.rotate);
        checkDarkenLevel();
    } else { //复位
        nowFalling.rotate = preRotate;
        insertBlock(preRotate);
    }
    return result;
}

/**
 * 抽取下一个方块，并输出到对应ui区
 */
void extractNextBlock() {
    nextBlock = randBetween(0, 6);
    nextRotate = randBetween(0, 3);
    //输出方块
    SetTextInPosition("  下一个方块  ", 80, 5, 7);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0 << 4 | blocks[nextBlock].color + 3);
    for (int i = 0; i < 4; i++) {
        MoveCursor(84, 7 + i);
        for (int j = 0; j < 4; j++) {
            printf(blocks[nextBlock].data[nextRotate][i][j] ? "■" : "  ");
        }
    }
}

/**
 * 尝试在顶部放置下一个方块
 * @return 是否成功放置
 */
bool appearBlock() {
    nowFalling.index = nextBlock;
    nowFalling.rotate = nextRotate; //从上次存储的“下一个方块”区读取
    int canRnd[21] = {0}, canRndCnt = -1;
    nowFalling.point[0] = 0;
    for (int i = 0; i <= 20; i++) {
        nowFalling.point[1] = i;
        if (checkBlock(nowFalling)) canRnd[++canRndCnt] = i; //枚举所有情况并得到可行域数组
    }
    if (canRndCnt <= 0) return false; //寄了
    int rnd = randBetween(0, canRndCnt); //抽一个可行解
    nowFalling.point[1] = canRnd[rnd];
    insertBlock(nowFalling.rotate);
    showBlock(nowFalling.rotate);
    return true;
}

/**
 * 检查是否可以消行；
 * 消行后统计分数；
 * 统计所有有效方块位置，防止后续惩罚清格子清了个寂寞
 */
void judgeLines() {
    int startLine = 0, nowRemoved = 0; //startLine用于小优化一下消行后需要更新的范围
    int comboScore[4] = {1, 2, 4, 8}; //combo对应的分数倍乘(>=4combo对应8)
    for (int i = 0; i < N; i++) { //枚举统计每行的有效方块个数
        validLine[i].count = 0;
        memset(validLine[i].validOnes, 0, sizeof validLine[i].validOnes);
        for (int j = 0; j < N; j++) if (currentMap[i][j].state) validLine[i].validOnes[validLine[i].count++] = j;
        if (validLine[i].count == 0) startLine++; //该行没方块，缩小更新范围
        else if (validLine[i].count == N) { //该行满行，消除下移
            for (int w = 0; w < N; w++) {
                if (w % 2 == 0) Sleep(10);
                AwaitSettingTextInPosition(26 + 2 * w, 3 + i, currentMap[i][w].color);
                printf("  ");
            }
            for (int p = i - 1; p >= startLine - 1; p--) {
                validLine[p + 1] = validLine[p];
                for (int w = 0; w < N; w++) {
                    currentMap[p + 1][w] = currentMap[p][w];
                    AwaitSettingTextInPosition(26 + 2 * w, 4 + p, currentMap[p + 1][w].color);
                    printf(currentMap[p + 1][w].state ? "■" : "  ");
                }
            }
            startLine++;
            nowRemoved++;//删除了一行
        }
    }
    if (nowRemoved > 0) { //统计加分并输出到ui
        int addScore = comboScore[min(3, nowRemoved - 1)] * nowRemoved * currentGameMode.multiply; //漏写了removedLine哈哈哈，我好蠢
        score += addScore;
        removedLines += nowRemoved; //统计删除的行
        if (enablePreview) refreshPreview();
        AwaitSettingTextInPosition(5, 7, 8);
        printScore(score);
        hidePreview(true);
        char added[100];
        sprintf(added, "Score Got:  %d！", addScore);
        showRandomActionHint(added);
        recoverDarkenLevel(false); //成功消行，恢复Darken值
        showRandomActionHint("");
        hidePreview(false);
    }
}

/**
 * 覆盖Darken列颜色，做动画 (down -> up)
 * @param down down
 * @param up up
 * @param color 方格颜色
 */
void animateDarkenCover(int down, int up, int color) {
    for (int i = down; i >= up; i--) {
        Sleep(2); //微小延时来实现类似于动画的效果
        AwaitSettingTextInPosition(24, 2 + i, color);
        printf("■");
        AwaitSettingTextInPosition(74, 2 + i, color);
        printf("■");
    }
}

/**
 * 恢复Darken值，做动画
 * @param level 起始Darken值 (level -> 0)
 * @param color 补上的方格颜色
 */
void animateDarkenRecover(int level, int color) {
    animateDarkenCover(level, beyondEnabled ? min(24, max(1, (level - max(-5, 9 - score / 12)))) : 1, color);
}

/**
 * 恢复Darken值
 * @param fail 成功消行: true, 超时: false
 */
void recoverDarkenLevel(bool fail) {
    animateDarkenRecover(darkLevel, fail ? 10 : 11); //10: light red, 11: light green
    animateDarkenRecover(darkLevel, 7); //恢复到初始颜色
    darkLevel = (beyondEnabled ? max(1, (darkLevel - max(-5, 9 - score / 12))) : 1) - 1;
    trialMove = 0;
}

/**
 * 图形化显示Darken值的减少
 */
void downDrawDarkenLevel() {
    AwaitSettingTextInPosition(24, 2 + darkLevel, 9);
    printf("■");
    AwaitSettingTextInPosition(74, 2 + darkLevel, 9);
    printf("■");
}

/**
 * 显示随机惩罚类型，方便玩家识别
 * @param action 类型文本
 */
void showRandomActionHint(char *action) {
    AwaitSettingTextInPosition(4, 1, 7);
    printf("%-24s", action);
}

/**
 * 随机消格
 * @return 是否消完格
 */
bool eraseBlockRandomly() {
    hidePreview(true);
    showRandomActionHint("Block Erased!");
    //找出所有符合条件的有效点
    int valid[N * N][2], cnt = 0, currentLine = N - 1, needLine =
            currentGameMode.mode == MODE_EZ.mode ? 3 :
            currentGameMode.mode == MODE_HD.mode ? 10 : N; //删格有效行数量  EZ: 3, HD: 10, In: 24
    for (int j = 0; j < needLine; j++) {
        if (validLine[currentLine].count == 0) needLine = min(N, needLine + 3); //跳过无效行，直到边界为止
        else {
            for (int p = 0; p < validLine[currentLine].count; p++) {
                valid[cnt][0] = currentLine;
                valid[cnt][1] = validLine[currentLine].validOnes[p];
                cnt++;
            }
        }
        currentLine--;
    }
    recoverDarkenLevel(true); //恢复Darken值
    for (int i = 0; i <= min(randBetween(3 * 3, 9 * 9), 6 + score / 3); i++) { //修复刷分数的bug
        if (cnt == 0) {
            showRandomActionHint("");
            hidePreview(false);
            return false; //给消没了
        }
        int randomIndex = randBetween(0, cnt - 1),
                *randomDelete = valid[randomIndex], delY = randomDelete[0], delX = randomDelete[1]; //抽一个幸运方块将其干没
        valid[randomIndex][0] = valid[cnt - 1][0];
        valid[randomIndex][1] = valid[cnt - 1][1];
        cnt--;
        currentMap[delY][delX].state = 0;
        currentMap[delY][delX].color = 15;
        MoveCursor(26 + 2 * delX, 3 + delY);
        printf("  ");
    }
    showRandomActionHint("");
    hidePreview(false);
    return cnt != 0;
}

/**
 * 随机交换两行
 * @return 是否成功交换
 */
bool exchangeRowRandomly() {
    hidePreview(true);
    int valid[N], cnt = 0;
    for (int j = nowFalling.point[0] + 4; j < N; j++) if (validLine[j].count > 0) valid[cnt++] = j; //从正在下落的方块的绝对底部到第N行找
    if (cnt > 1) {
        showRandomActionHint("Line Blocks Exchanged!");
        int exchangeA = randBetween(0, cnt - 1), exchangeB = randBetween(0, cnt - 2);
        if (exchangeB >= exchangeA) exchangeB++; //防止A和B自交(?
        exchangeA = valid[exchangeA];
        exchangeB = valid[exchangeB];
        ValidLine tmpValid = validLine[exchangeA];
        validLine[exchangeA] = validLine[exchangeB];
        validLine[exchangeB] = tmpValid;
        recoverDarkenLevel(true); //恢复Darken值
        for (int p = 0; p < N; p++) {
            Point tmpPoint = currentMap[exchangeA][p];
            currentMap[exchangeA][p] = currentMap[exchangeB][p];
            currentMap[exchangeB][p] = tmpPoint;
            if (currentMap[exchangeA][p].state == currentMap[exchangeB][p].state &&
                currentMap[exchangeA][p].color == currentMap[exchangeB][p].color)
                continue; //如果一模一样就没必要动画了
            Sleep(1);
            AwaitSettingTextInPosition(26 + 2 * p, 3 + exchangeA, currentMap[exchangeA][p].color);
            printf(currentMap[exchangeA][p].state ? "■" : "  ");
            Sleep(1);
            AwaitSettingTextInPosition(26 + 2 * p, 3 + exchangeB, currentMap[exchangeB][p].color);
            printf(currentMap[exchangeB][p].state ? "■" : "  ");
        }
        showRandomActionHint("");
        hidePreview(false);
        return true;
    }
    hidePreview(false);
    return false;
}

/**
 * 随机修改速度
 */
void changeSpeedRandomly() {
    hidePreview(true);
    bool down = randBetween(0, 9) < 5;
    showRandomActionHint(down ? "Speed Down!" : "Speed Up!");
    recoverDarkenLevel(true); //恢复Darken值
    speedMultiply = down ? (double) randBetween(1250, 1562) / 1000
            : (double) randBetween(250, 500) / 1000; //0.64~0.80, 2.00~4.00倍速，精度0.01
    if (enablePreview) refreshPreview();
    speedMultiplyEnabledTime = GetTickCount();
    showRandomActionHint("");
    hidePreview(false);
}

/**
 * 检查是否需要扣除Darken值
 */
void checkDarkenLevel() {
    double increase[3] = {0.16, 0.32, 0.64};
    if (trialMove == max(2, (int) (18 - (double) score / (100 * increase[currentGameMode.mode])))) { //达到阈值
        trialMove = 0;
        darkLevel++;
        if (darkLevel < N) downDrawDarkenLevel();
        else { //记录一次惩罚
            if(challengeModeEnabled){
                challengeModeFault ++;
                if(challengeModeFault >= 3){ //挑战失败
                    forceEndGame = true;
                    endGame(false);
                    return;
                }
            }
            speedMultiply = 1.0; //复位速度
            if (currentGameMode.mode == MODE_IN.mode) {
                int type = randBetween(0, 100);
                if (type <= 30 && exchangeRowRandomly()) return;
                if (type <= 60 || score >= 1000) { //防止刷分！！！！！
                    changeSpeedRandomly();
                    return;
                }
            }
            if (!eraseBlockRandomly()) {
                score += 100; //能消完也是个神仙了
                AwaitSettingTextInPosition(5, 7, 8);
                printScore(score);
            }
        }
    } else trialMove++; //继续累计
}

/**
 * 输出存入的方格到对应ui区
 */
void showSavedBlock() {
    SetTextInPosition("  存入的方块  ", 80, 18, 7);
    SetTextInPosition("              ", 80, 22, 8);
    SetTextInPosition("              ", 80, 24, 8);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0 << 4 | blocks[savedBlock].color + 3);
    for (int i = 0; i < 4; i++) {
        MoveCursor(84, 21 + i);
        for (int j = 0; j < 4; j++) {
            printf(blocks[savedBlock].data[savedRotate][i][j] ? "■" : "  ");
        }
    }
}

/**
 * 保存方块
 */
void saveBlock() {
    if (savedBlock != -1 && savedRotate != -1) {
        Fall what = {savedBlock, savedRotate, nowFalling.point[0], nowFalling.point[1]};
        clearBlock(nowFalling.rotate);
        if (checkBlock(what)) { //能放置，就交换
            savedBlock = nowFalling.index;
            savedRotate = nowFalling.rotate;
            hideBlock(nowFalling.rotate);
            nowFalling = what;
            insertBlock(nowFalling.rotate);
            showBlock(nowFalling.rotate);
            showSavedBlock();
        } else insertBlock(nowFalling.rotate);
    } else { //没存方块，直接存入并显示下一个方块
        savedBlock = nowFalling.index;
        savedRotate = nowFalling.rotate;
        clearBlock(nowFalling.rotate);
        hideBlock(nowFalling.rotate);
        appearBlock();
        extractNextBlock();
        showSavedBlock();
    }
}