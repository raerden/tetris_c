#ifndef TETRIS_BACKEND_H
#define TETRIS_BACKEND_H

#include <time.h>
#include <stdlib.h>

#define SPAWN_X 4
#define SPAWN_Y 0
#define FIGURE_FIELD_SIZE 4


typedef enum {
    FSM_Start,
    FSM_Spawn,
    FSM_Moving,   //ожидание ввода, перемещение фигуры игроком
    FSM_Shifting, //Сдвиг фигуры вниз по таймеру
    FSM_Attaching,
    FSM_PauseGame,
    FSM_GameOver
} FSM_State_t;

typedef struct {
  int **field;
  int **next;
  int next_size;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
  int **figure;
  int figure_size; //размер фигуры, для упрощения поворотов
  int pos_x;
  int pos_y;
  long long time;
} TetrisGameInfo_t;

long long get_time();
int **create_matrix(int row, int col);
void free_matrix(int **matrix, int size);
void copy_matrix(int **dst, int **scr, int row, int col);
void GenerateNextFigure(TetrisGameInfo_t *TetrisGameInfo);

#endif