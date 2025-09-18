#ifndef TETRIS_BACKEND_H
#define TETRIS_BACKEND_H

#include <time.h>
#include <stdlib.h>

#define SPAWN_X 4;
#define SPAWN_Y 0;

long long get_time();
int **create_matrix(int row, int col);
void free_matrix(int **matrix, int size);

#endif