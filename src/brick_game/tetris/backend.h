#ifndef TETRIS_BACKEND_H
#define TETRIS_BACKEND_H

#include <time.h>
#include <stdlib.h>
#include "../../brick_game.h"

#define SPAWN_X 4
#define SPAWN_Y 0
#define FIELD_W     10
#define FIELD_H     20
#define FIGURE_FIELD_SIZE 4
#define INITIAL_SPEED 200 // установить на 1000
#define GETCH_WAIT 15


typedef enum {
    FSM_Start,
    FSM_Spawn,
    FSM_Moving,   //ожидание ввода, перемещение фигуры игроком
    FSM_Shifting, //Сдвиг фигуры вниз по таймеру
    FSM_Attaching,
    FSM_GamePause,
    FSM_Terminate,
} FSM_State_t;

typedef struct {
  FSM_State_t state;
  int **field;
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
  int figure_w; //размер фигуры, для логики поворотов прямоугольниками
  int figure_h; 
  int pos_x; // левый верхний угол матрицы с фигурой
  int pos_y;
  long long time;
} TetrisGameInfo_t;

long long get_time();
void delay_ms(int milliseconds);
int **createMatrix(int row, int col);
void freeMatrix(int **matrix, int size);
void copyMatrix(int **dst, int **scr, int row, int col);
void clearMatrix(int **matrix, int row, int col);
TetrisGameInfo_t *getTetrisGameInfo();
void setState(FSM_State_t state);
bool currentState(FSM_State_t state);
void terminate(GameInfo_t *gameInfo);
void genNextFigure();
void spawnFigure();
void copyFigureToField();
void tetrisToGameInfo(GameInfo_t *GameInfo);
bool isCollided(int **figure, int width, int heigth, int pos_x, int pos_y);
void swapInt(int *a, int *b);
void rotateFigure();
void moveLeft();
void moveRigth();
void dropDown();
void attachFigure();
void deleteLines();
void gamePause();

#endif