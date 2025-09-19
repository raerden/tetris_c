#include "backend.h"

long long get_time() {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
  return (long long)ts.tv_sec * 1000 + ts.tv_nsec / 1000;
}

int **create_matrix(int row, int col) {
    int **matrix = (int **)calloc(row, sizeof(int*));
    for(int i=0; i < row; i++) {
        matrix[i] = (int *)calloc(col, sizeof(int));
        if (matrix[i] == NULL) {
            free_matrix(matrix, row);
            break;
        }
    }
    return matrix;
}

void free_matrix(int **matrix, int size) {
    if (matrix != NULL) {
        for (int i = 0; i < size; i++)
            free(matrix[i]);
        free(matrix);
    }
    matrix = NULL;
}

void copy_matrix(int **dst, int **src, int row, int col) {
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            dst[i][j] = src[i][j];
}

void GenerateNextFigure(TetrisGameInfo_t *TetrisGameInfo) {
    //  Номер фигуры также является цветом.
    int figure = 1 + rand() % 7; // 1 - 7

    for (int i = 0; i < FIGURE_FIELD_SIZE; i++)
        for (int j = 0; j < FIGURE_FIELD_SIZE; j++)
            TetrisGameInfo->next[i][j] = 0;

    switch (figure)
    {
    case 1: // Палка.
        TetrisGameInfo->next[0][0] = figure;
        TetrisGameInfo->next[0][1] = figure;
        TetrisGameInfo->next[0][2] = figure;
        TetrisGameInfo->next[0][3] = figure;
        TetrisGameInfo->next_size = 4;
        break;
    case 2: // Г - правая (оранжевая) повернута вниз. Прижата к верхней границе квадрата
        TetrisGameInfo->next[0][0] = figure;
        TetrisGameInfo->next[0][1] = figure;
        TetrisGameInfo->next[0][2] = figure;
        TetrisGameInfo->next[1][2] = figure;
        TetrisGameInfo->next_size = 3;
        break;
    case 3: // Г - левая (желтая)
        TetrisGameInfo->next[0][0] = figure;
        TetrisGameInfo->next[0][1] = figure;
        TetrisGameInfo->next[0][2] = figure;
        TetrisGameInfo->next[1][0] = figure;
        TetrisGameInfo->next_size = 3;
        break;
    case 4: // квадрат
        TetrisGameInfo->next[0][0] = figure;
        TetrisGameInfo->next[0][1] = figure;
        TetrisGameInfo->next[1][0] = figure;
        TetrisGameInfo->next[1][1] = figure;
        TetrisGameInfo->next_size = 2;
        break;
    case 5: // z-правая  зеленая
        TetrisGameInfo->next[0][1] = figure;
        TetrisGameInfo->next[0][2] = figure;
        TetrisGameInfo->next[1][0] = figure;
        TetrisGameInfo->next[1][1] = figure;
        TetrisGameInfo->next_size = 3;
        break;
    case 6: // T  синяя
        TetrisGameInfo->next[0][0] = figure;
        TetrisGameInfo->next[0][1] = figure;
        TetrisGameInfo->next[0][2] = figure;
        TetrisGameInfo->next[1][1] = figure;
        TetrisGameInfo->next_size = 3;
        break;
    case 7: // z-левая   пурпурная
        TetrisGameInfo->next[0][0] = figure;
        TetrisGameInfo->next[0][1] = figure;
        TetrisGameInfo->next[1][1] = figure;
        TetrisGameInfo->next[1][2] = figure;
        TetrisGameInfo->next_size = 3;
        break;
    default:
        break;
    }
}