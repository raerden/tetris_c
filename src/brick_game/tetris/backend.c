#include "backend.h"

long long get_time() {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
  return (long long)ts.tv_sec * 1000 + ts.tv_nsec / 1000000LL;
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

void clear_matrix(int **matrix, int row, int col) {
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            matrix[i][j] = 0;
}

TetrisGameInfo_t *getTetrisGameInfo() {
    static TetrisGameInfo_t TetrisGameInfo = {0};
    if (TetrisGameInfo.field == NULL) {
        TetrisGameInfo.field = create_matrix(FIELD_H, FIELD_W);
        TetrisGameInfo.next = create_matrix(FIGURE_FIELD_SIZE, FIGURE_FIELD_SIZE);
        TetrisGameInfo.figure = create_matrix(FIGURE_FIELD_SIZE, FIGURE_FIELD_SIZE);
        TetrisGameInfo.figure_tmp = create_matrix(FIGURE_FIELD_SIZE, FIGURE_FIELD_SIZE);
    }
    return &TetrisGameInfo;
}

void terminate(GameInfo_t *gameInfo) {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();

    free_matrix(gameInfo->field, FIELD_H);
    free_matrix(TetrisGameInfo->field, FIELD_H);
    free_matrix(TetrisGameInfo->next, FIGURE_FIELD_SIZE);
    free_matrix(TetrisGameInfo->figure, FIGURE_FIELD_SIZE);
    free_matrix(TetrisGameInfo->figure_tmp, FIGURE_FIELD_SIZE);
}

void GenerateNextFigure() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    //  Номер фигуры также является её цветом.
    int figure = 1 + rand() % 7;

    for (int i = 0; i < FIGURE_FIELD_SIZE; i++)
        for (int j = 0; j < FIGURE_FIELD_SIZE; j++)
            TetrisGameInfo->next[i][j] = 0;
//figure = 3;
    switch (figure)
    {
    case 1: // Палка.
        TetrisGameInfo->next[0][0] = figure;
        TetrisGameInfo->next[0][1] = figure;
        TetrisGameInfo->next[0][2] = figure;
        TetrisGameInfo->next[0][3] = figure;
        TetrisGameInfo->next_w = 4;
        TetrisGameInfo->next_h = 1;
        break;
    case 2: // Г - правая (оранжевая) повернута вниз. Прижата к верхней границе квадрата
        TetrisGameInfo->next[0][0] = figure;
        TetrisGameInfo->next[0][1] = figure;
        TetrisGameInfo->next[0][2] = figure;
        TetrisGameInfo->next[1][2] = figure;
        TetrisGameInfo->next_w = 3;
        TetrisGameInfo->next_h = 2;
        break;
    case 3: // Г - левая (желтая)
        TetrisGameInfo->next[0][0] = figure;
        TetrisGameInfo->next[0][1] = figure;
        TetrisGameInfo->next[0][2] = figure;
        TetrisGameInfo->next[1][0] = figure;
        TetrisGameInfo->next_w = 3;
        TetrisGameInfo->next_h = 2;
        break;
    case 4: // квадрат
        TetrisGameInfo->next[0][0] = figure;
        TetrisGameInfo->next[0][1] = figure;
        TetrisGameInfo->next[1][0] = figure;
        TetrisGameInfo->next[1][1] = figure;
        TetrisGameInfo->next_w = 2;
        TetrisGameInfo->next_h = 2;
        break;
    case 5: // z-правая  зеленая
        TetrisGameInfo->next[0][1] = figure;
        TetrisGameInfo->next[0][2] = figure;
        TetrisGameInfo->next[1][0] = figure;
        TetrisGameInfo->next[1][1] = figure;
        TetrisGameInfo->next_w = 3;
        TetrisGameInfo->next_h = 2;
        break;
    case 6: // T  синяя
        TetrisGameInfo->next[0][0] = figure;
        TetrisGameInfo->next[0][1] = figure;
        TetrisGameInfo->next[0][2] = figure;
        TetrisGameInfo->next[1][1] = figure;
        TetrisGameInfo->next_w = 3;
        TetrisGameInfo->next_h = 2;
        break;
    case 7: // z-левая   пурпурная
        TetrisGameInfo->next[0][0] = figure;
        TetrisGameInfo->next[0][1] = figure;
        TetrisGameInfo->next[1][1] = figure;
        TetrisGameInfo->next[1][2] = figure;
        TetrisGameInfo->next_w = 3;
        TetrisGameInfo->next_h = 2;
        break;
    default:
        break;
    }
}

void spawnFigure() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    copy_matrix(TetrisGameInfo->figure, TetrisGameInfo->next, FIGURE_FIELD_SIZE, FIGURE_FIELD_SIZE);
    TetrisGameInfo->figure_w = TetrisGameInfo->next_w;
    TetrisGameInfo->figure_h = TetrisGameInfo->next_h;
    TetrisGameInfo->pos_x = SPAWN_X - (TetrisGameInfo->figure_w == 4 ? 1 : 0);
    TetrisGameInfo->pos_y = SPAWN_Y;
    GenerateNextFigure();
    TetrisGameInfo->status = FSM_Moving;
    TetrisGameInfo->time = get_time();
    printlog("spawnFigure() set to FSM_Moving");
}

void copyTetrominoToField(GameInfo_t *GameInfo) {
    const TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    for (int y = 0; y < TetrisGameInfo->figure_h; y++)
        for (int x = 0; x < TetrisGameInfo->figure_w; x++) {
            int field_x = x + TetrisGameInfo->pos_x;
            int field_y = y + TetrisGameInfo->pos_y;
//mvprintw(25+y, 1, "x(col)=%d, y(row)=%d, field_x(col)=%d, field_y(row)=%d", x, y, field_x, field_y);
            if (TetrisGameInfo->figure[y][x] > 0 && field_x < FIELD_W &&  field_y < FIELD_H)
                GameInfo->field[field_y][field_x] = TetrisGameInfo->figure[y][x];
        }
}

void TetrisToGameInfo(GameInfo_t *GameInfo) {
    const TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    copy_matrix(GameInfo->field, TetrisGameInfo->field, FIELD_H, FIELD_W);
    copyTetrominoToField(GameInfo);
    GameInfo->next = TetrisGameInfo->next;
    GameInfo->score = TetrisGameInfo->score;
    GameInfo->high_score = TetrisGameInfo->high_score;
    GameInfo->level = TetrisGameInfo->level;
    GameInfo->speed = TetrisGameInfo->speed;
    GameInfo->pause = TetrisGameInfo->pause;
}