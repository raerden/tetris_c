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
#define INITIAL_SPEED 200


typedef enum {
    FSM_Start,
    FSM_Spawn,
    FSM_Moving,   //ожидание ввода, перемещение фигуры игроком
    FSM_Shifting, //Сдвиг фигуры вниз по таймеру
    FSM_Attaching,
    FSM_GamePause,
    FSM_GameWin,
    FSM_GameOver,
    FSM_Terminate,
} FSM_State_t;

typedef struct {
  FSM_State_t status;
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
int **create_matrix(int row, int col);
void free_matrix(int **matrix, int size);
void copy_matrix(int **dst, int **scr, int row, int col);
void clear_matrix(int **matrix, int row, int col);
TetrisGameInfo_t *getTetrisGameInfo();
void terminate(GameInfo_t *gameInfo);
void GenerateNextFigure();
void spawnFigure();
void copyTetrominoToField(GameInfo_t *GameInfo);
void TetrisToGameInfo(GameInfo_t *GameInfo);

#endif