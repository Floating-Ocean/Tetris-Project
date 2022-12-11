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

#include <pthread.h>
#include <conio.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <wtsapi32.h>
#include <richedit.h>

#ifndef TETRIS_STRUCTS_H
#define TETRIS_STRUCTS_H

#endif //TETRIS_STRUCTS_H

typedef struct {
    int index, rotate, point[2];
} Fall;

typedef struct {
    int color, data[4][4][4]; //方块点阵
} Block;

typedef struct {
    int color, state;
} Point;

typedef struct {
    int count, validOnes[24];
} ValidLine;

typedef struct {
    char *modeName;
    int mode, speed, multiply;
} GameMode;

typedef struct {
    DWORD previousTimeMills, currentTimeMills;
} Timer;