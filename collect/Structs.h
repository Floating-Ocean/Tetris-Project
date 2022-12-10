#include <pthread.h>
#include <conio.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <wtsapi32.h>

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