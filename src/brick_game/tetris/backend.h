#ifndef TETRIS_BACKEND_H
#define TETRIS_BACKEND_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../../brick_game.h"

#define SPAWN_X 4
#define SPAWN_Y 0
#define INITIAL_SPEED 1250
#define LEVEL_TRESHOLD 600
#define MAX_LEVEL 10
#define SECRET_CHECKSUM 0xA512
#define BEST_SCORE_FILE "score.dt"

typedef enum {
  FSM_Start,
  FSM_Spawn,
  FSM_Moving,  // перемещение фигуры игроком или таймером
  FSM_GamePause
} FSM_State_t;

typedef struct {
  FSM_State_t state;
  int **field;
  int **field_front;  // поле для фронтенда
  int **next;
  int next_w;
  int next_h;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
  int **figure;
  int **figure_tmp;
  int figure_w;  // размер фигуры, для логики поворотов прямоугольниками
  int figure_h;
  int pos_x;  // левый верхний угол матрицы с фигурой
  int pos_y;
  long long time;
  bool fastDrop;
} TetrisGameInfo_t;

typedef struct {
  int score;
  int checksum;
} score_t;

TetrisGameInfo_t *getTetrisGameInfo(void);
GameInfo_t tetrisToGameInfo(void);

void startGame(void);
void terminateGame(void);

void setState(FSM_State_t state);
bool currentState(FSM_State_t state);

void gamePause(void);
void gameWin(void);
void gameOver(void);

int calcSpeed(int level);
int calcScore(int full_rows);

bool isFullRow(int **field, int row);
bool isCollided(int **figure, int heigth, int width, int pos_y, int pos_x);
bool isTimerExpired(void);

void removeRow(int **field, int row);
void deleteLines(void);

int **createMatrix(int row, int col);
void freeMatrix(int **matrix, int size);
void copyMatrix(int **dst, int **scr, int row, int col);
void clearMatrix(int **matrix, int row, int col);

void genNextFigure(void);
void spawnFigure(void);

void swapInt(int *a, int *b);

void moveLeft(void);
void moveRigth(void);
void moveDown(void);
void dropDown(void);
void rotateFigure(void);

void attachFigure(void);
void deleteLines(void);

int genCheckSum(int num);
void saveScore(int score);
int loadScore(void);
void tetrisLogo(void);

#endif
