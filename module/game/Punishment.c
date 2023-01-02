/*
 * Copyright (C) 2022-2023 Floating Ocean
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
 * 减分，范围为1-128
 */
void minusScore() {
    hidePreview(true);
    int minus = randBetween(1, 128);
    char notice[78];
    sprintf(notice, "Score Lost: %d...  What a Pity!", minus);
    showRandomActionHint(notice);
    score -= minus;
    score = max(0, score);
    AwaitSettingTextInPosition(5, 7, COLOR_SUB_TEXT);
    printScore(score);
    Sleep(500);
    recoverDarkenLevel(true); //恢复Darken值
    showRandomActionHint("");
    hidePreview(false);
}

/**
 * 在中间插入挡板作为左右分隔
 */
void insertBarrier() {
    hidePreview(true);
    showRandomActionHint("Barrier Inserted!");
    bool straight = nowFalling.point[1] < N / 2 - 4 || nowFalling.point[1] > N / 2;
    int insertLeft = straight ? N / 2 - 1 : nowFalling.point[1] - 1, insertRight = straight ? N / 2 : nowFalling.point[1] + 4;
    for(int i=1;i < N - 1;i++) {
        bool insert = currentMap[i + 1][insertLeft].state == 0 && currentMap[i + 1][insertRight].state == 0;
        if (insert) {
            Sleep(10);
            currentMap[i][insertLeft].state = 1;
            currentMap[i][insertRight].state = 1;
            currentMap[i][insertLeft].color = COLOR_MAIN_TEXT;
            currentMap[i][insertRight].color = COLOR_MAIN_TEXT;
            AwaitSettingTextInPosition(26 + 2 * insertLeft, 3 + i, currentMap[i][insertLeft].color);
            printf(currentMap[i][insertLeft].state ? "■" : "  ");
            AwaitSettingTextInPosition(26 + 2 * insertRight, 3 + i, currentMap[i][insertRight].color);
            printf(currentMap[i][insertRight].state ? "■" : "  ");
        } else break;
    }
    recoverDarkenLevel(true); //恢复Darken值
    showRandomActionHint("");
    hidePreview(false);
}

/**
 * 左右镜像翻转方块，不影响操作
 */
void mirrorPartially() {
    hidePreview(true);
    showRandomActionHint("Partially Mirrored!");
    for (int i = nowFalling.point[0] + 4; i < N; i++) {
        for (int j = 0; j < N / 2; j++) {
            bool animate = (currentMap[i][j].state + currentMap[i][N - j - 1].state) > 0;
            Point tmp = currentMap[i][j];
            currentMap[i][j] = currentMap[i][N - j - 1];
            currentMap[i][N - j - 1] = tmp;
            AwaitSettingTextInPosition(26 + 2 * j, 3 + i, currentMap[i][j].color);
            if(animate) Sleep(10);
            printf(currentMap[i][j].state ? "■" : "  ");
            AwaitSettingTextInPosition(26 + 2 * (N - j - 1), 3 + i, currentMap[i][j].color);
            if(animate) Sleep(10);
            printf(currentMap[i][N - j - 1].state ? "■" : "  ");
        }
    }
    recoverDarkenLevel(true); //恢复Darken值
    showRandomActionHint("");
    hidePreview(false);
}

/**
 * 将游玩页面水平&垂直翻转，持续至下一次消行
 */
void mirrorTotally() {
    hidePreview(true);
    showRandomActionHint("Gravity Mirrored!");
    hideBlock(nowFalling.rotate);
    for (int i = 0; i < N; i++) {
        int cnt = 0;
        for (int j = 0; j < N; j++) {
            cnt += currentMap[i][j].state;
            AwaitSettingTextInPosition(26 + 2 * j, 3 + i, currentMap[i][j].color);
            printf("  ");
        }
        if(cnt > 0) Sleep(10);
    }
    mirrorEnabled = !mirrorEnabled;
    for (int i = 0; i < N; i++) {
        int cnt = 0;
        for (int j = 0; j < N; j++) {
            cnt += currentMap[i][j].state;
            if (mirrorEnabled) AwaitSettingTextInPosition(26 + 2 * (23 - j), 3 + 23 - i, currentMap[i][j].color);
            else AwaitSettingTextInPosition(26 + 2 * j, 3 + i, currentMap[i][j].color);
            printf(currentMap[i][j].state ? "■" : "  ");
        }
        if(cnt > 0) Sleep(10);
    }
    showBlock(nowFalling.rotate);
    if (mirrorEnabled) recoverDarkenLevel(true); //恢复Darken值
    showRandomActionHint("");
    hidePreview(false);
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
            currentGameMode.mode == MODE_EZ.mode ? 8 :
            currentGameMode.mode == MODE_HD.mode ? 16 : N; //删格有效行数量  EZ: 8, HD: 16, In: 24
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
        if (mirrorEnabled) MoveCursor(26 + 2 * (23 - delX), 3 + 23 - delY);
        else MoveCursor(26 + 2 * delX, 3 + delY);
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
            if (mirrorEnabled) AwaitSettingTextInPosition(26 + 2 * (23 - p), 3 + 23 - exchangeA, currentMap[exchangeA][p].color);
            else AwaitSettingTextInPosition(26 + 2 * p, 3 + exchangeA, currentMap[exchangeA][p].color);
            printf(currentMap[exchangeA][p].state ? "■" : "  ");
            Sleep(1);
            if (mirrorEnabled) AwaitSettingTextInPosition(26 + 2 * (23 - p), 3 + 23 - exchangeB, currentMap[exchangeB][p].color);
            else AwaitSettingTextInPosition(26 + 2 * p, 3 + exchangeB, currentMap[exchangeB][p].color);
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
    if (beyondEnabled)
        speedMultiply = down ? (double) randBetween(1041, 1388) / 1000
                             : (double) randBetween(156, 400) / 1000; //0.72~0.96, 2.50~6.40倍速，精度0.01
    else
        speedMultiply = down ? (double) randBetween(1250, 1562) / 1000
                             : (double) randBetween(250, 500) / 1000; //0.64~0.80, 2.00~4.00倍速，精度0.01
    if (enablePreview) refreshPreview();
    speedMultiplyEnabledTime = GetTickCount();
    showRandomActionHint("");
    hidePreview(false);
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
    animateDarkenRecover(darkLevel, fail ? COLOR_FAULT : COLOR_PASS); //状态颜色提示
    animateDarkenRecover(darkLevel, COLOR_MAIN_TEXT); //恢复到初始颜色
    darkLevel = (beyondEnabled ? max(1, (darkLevel - max(-5, 9 - score / 12))) : 1) - 1;
    trialMove = 0;
}

/**
 * 图形化显示Darken值的减少
 */
void downDrawDarkenLevel() {
    AwaitSettingTextInPosition(24, 2 + darkLevel, COLOR_MILD);
    printf("■");
    AwaitSettingTextInPosition(74, 2 + darkLevel, COLOR_MILD);
    printf("■");
}

/**
 * 显示随机惩罚类型，方便玩家识别
 * @param action 类型文本
 */
void showRandomActionHint(char *action) {
    AwaitSettingTextInPosition(4, 1, COLOR_MAIN_TEXT);
    printf("%-24s", action);
}


/**
 * 检查是否需要扣除Darken值
 */
void checkDarkenLevel(bool *ended) {
    double increase[3] = {0.16, 0.32, 0.64};
    if (trialMove == max(2, (int) (18 - (double) score / (100 * increase[currentGameMode.mode])))) { //达到阈值
        trialMove = 0;
        darkLevel++;
        if (darkLevel < N) downDrawDarkenLevel();
        else { //记录一次惩罚
            if (challengeModeEnabled) {
                challengeModeFault++;
                if (challengeModeFault >= 3) { //挑战失败
                    forceEndGame = true;
                    *ended = true;
                    endGame(false);
                    return;
                }
            }
            speedMultiply = 1.0; //复位速度
            if (currentGameMode.mode == MODE_IN.mode) {
                bool toErase = false;
                int type = randBetween(0, 10000);
                if (type <= 2800 && exchangeRowRandomly()); //nothing
                else if(type <= 3600 && !mirrorEnabled) mirrorTotally();
                else if(type <= 4400) mirrorPartially();
                else if(type <= 5200) insertBarrier();
                else if(type <= 5300) minusScore();
                else if (type <= 7600 || score >= 1000) changeSpeedRandomly();  //防止刷分！！！！！
                else toErase = true;
                if(!toErase) return;
            }
            if (!eraseBlockRandomly()) {
                score += beyondEnabled ? 25 : 100; //能消完也是个神仙了
                AwaitSettingTextInPosition(5, 7, COLOR_SUB_TEXT);
                printScore(score);
            }
        }
    } else trialMove++; //继续累计
}