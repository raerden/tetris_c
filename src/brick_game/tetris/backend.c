#include "backend.h"

long long getTime() {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
  return (long long)ts.tv_sec * 1000 + ts.tv_nsec / 1000000LL;
}

int **createMatrix(int row, int col) {
    int **matrix = (int **)calloc(row, sizeof(int*));
    for(int i=0; i < row; i++) {
        matrix[i] = (int *)calloc(col, sizeof(int));
        if (matrix[i] == NULL) {
            freeMatrix(matrix, row);
            break;
        }
    }
    return matrix;
}

void freeMatrix(int **matrix, int size) {
    if (matrix != NULL) {
        for (int i = 0; i < size; i++)
            free(matrix[i]);
        free(matrix);
    }
    matrix = NULL;
}

void copyMatrix(int **dst, int **src, int row, int col) {
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            dst[i][j] = src[i][j];
}

void clearMatrix(int **matrix, int row, int col) {
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            matrix[i][j] = 0;
}

TetrisGameInfo_t *getTetrisGameInfo() {
    static TetrisGameInfo_t TetrisGameInfo = {0};
    if (TetrisGameInfo.field == NULL) {
        TetrisGameInfo.field = createMatrix(FIELD_H, FIELD_W);
        TetrisGameInfo.next = createMatrix(FIGURE_FIELD_SIZE, FIGURE_FIELD_SIZE);
        TetrisGameInfo.figure = createMatrix(FIGURE_FIELD_SIZE, FIGURE_FIELD_SIZE);
        TetrisGameInfo.figure_tmp = createMatrix(FIGURE_FIELD_SIZE, FIGURE_FIELD_SIZE);
    }
    return &TetrisGameInfo;
}

void setState(FSM_State_t state) {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    if (state != TetrisGameInfo->state)
        TetrisGameInfo->state = state;
}

bool currentState(FSM_State_t state) {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    return TetrisGameInfo->state == state;
}

void terminateGame(GameInfo_t *gameInfo) {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    saveScore(TetrisGameInfo->high_score);
    freeMatrix(gameInfo->field, FIELD_H);
    freeMatrix(TetrisGameInfo->field, FIELD_H);
    freeMatrix(TetrisGameInfo->next, FIGURE_FIELD_SIZE);
    freeMatrix(TetrisGameInfo->figure, FIGURE_FIELD_SIZE);
    freeMatrix(TetrisGameInfo->figure_tmp, FIGURE_FIELD_SIZE);
}

void genNextFigure() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    clearMatrix(TetrisGameInfo->next, FIGURE_FIELD_SIZE, FIGURE_FIELD_SIZE);
    //  Номер фигуры также является её цветом.
    int figure = 1 + rand() % 7;
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
    copyMatrix(TetrisGameInfo->figure, TetrisGameInfo->next, FIGURE_FIELD_SIZE, FIGURE_FIELD_SIZE);
    TetrisGameInfo->figure_w = TetrisGameInfo->next_w;
    TetrisGameInfo->figure_h = TetrisGameInfo->next_h;
    TetrisGameInfo->pos_x = SPAWN_X - (TetrisGameInfo->figure_w == 4 ? 1 : 0);
    TetrisGameInfo->pos_y = SPAWN_Y;
    // проверить коллизию из фигуры и перевести в FSM_Start
    if (isCollided(TetrisGameInfo->figure, TetrisGameInfo->figure_h, TetrisGameInfo->figure_w, TetrisGameInfo->pos_y, TetrisGameInfo->pos_x)) {
        TetrisGameInfo->pause = 3;// Поражение
        setState(FSM_Start);
        printlog("spawnFigure() set to FSM_Start");
    } else {
        genNextFigure();
        setState(FSM_Moving);
        TetrisGameInfo->time = getTime();
        printlog("spawnFigure() set to FSM_Moving");
    }
}

void copyFigureToField() {
    const TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    for (int y = 0; y < TetrisGameInfo->figure_h; y++)
        for (int x = 0; x < TetrisGameInfo->figure_w; x++) {
            int field_x = x + TetrisGameInfo->pos_x;
            int field_y = y + TetrisGameInfo->pos_y;
            if (TetrisGameInfo->figure[y][x] > 0 && field_x < FIELD_W && field_y < FIELD_H)
                TetrisGameInfo->field[field_y][field_x] = TetrisGameInfo->figure[y][x];
        }
}

void tetrisToGameInfo(GameInfo_t *GameInfo) {
    const TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    copyMatrix(GameInfo->field, TetrisGameInfo->field, FIELD_H, FIELD_W);
    //copyFigureToField(GameInfo);
    for (int y = 0; y < TetrisGameInfo->figure_h; y++)
        for (int x = 0; x < TetrisGameInfo->figure_w; x++) {
            int field_x = x + TetrisGameInfo->pos_x;
            int field_y = y + TetrisGameInfo->pos_y;
            if (TetrisGameInfo->figure[y][x] > 0 && field_x < FIELD_W && field_y < FIELD_H)
                GameInfo->field[field_y][field_x] = TetrisGameInfo->figure[y][x];
        }

    GameInfo->next = TetrisGameInfo->next;
    GameInfo->score = TetrisGameInfo->score;
    GameInfo->high_score = TetrisGameInfo->high_score;
    GameInfo->level = TetrisGameInfo->level;
    GameInfo->speed = TetrisGameInfo->speed;
    GameInfo->pause = TetrisGameInfo->pause;
}

bool isCollided(int **figure, int heigth, int width, int pos_y, int pos_x) {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    bool res = false;

    for (int y = 0; res == false && y < heigth; y++)
        for (int x = 0; res == false && x < width; x++) 
            if (figure[y][x] > 0 && TetrisGameInfo->field[y + pos_y][x + pos_x] > 0)
                res = true;
    
    return res;
}

void swapInt(int *a, int *b) {
    int t = *b;
    *b = *a;
    *a = t;
}

void rotateFigure() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    clearMatrix(TetrisGameInfo->figure_tmp, FIGURE_FIELD_SIZE, FIGURE_FIELD_SIZE);
    //поворот направо
    //x идет в строку y
    //y идет в x, но с конца
    for (int y = 0; y < TetrisGameInfo->figure_h; y++) {
        for (int x = 0; x < TetrisGameInfo->figure_w; x++) {
            TetrisGameInfo->figure_tmp[x][TetrisGameInfo->figure_h - 1 - y] = TetrisGameInfo->figure[y][x];
        }
    }

    //Коллизия с правой стенкой. Смещаем фигуру влево.
    int new_x;
    if (TetrisGameInfo->pos_x + TetrisGameInfo->figure_h > FIELD_W) 
        new_x = TetrisGameInfo->pos_x - (TetrisGameInfo->pos_x + TetrisGameInfo->figure_h - FIELD_W);
    else
        new_x = TetrisGameInfo->pos_x;

    if (!isCollided(TetrisGameInfo->figure_tmp, TetrisGameInfo->figure_w, TetrisGameInfo->figure_h, TetrisGameInfo->pos_y, new_x) && \
            TetrisGameInfo->pos_y + TetrisGameInfo->figure_w <= FIELD_H) {
        swapInt(&TetrisGameInfo->figure_w, &TetrisGameInfo->figure_h);
        copyMatrix(TetrisGameInfo->figure, TetrisGameInfo->figure_tmp, FIGURE_FIELD_SIZE, FIGURE_FIELD_SIZE);
        TetrisGameInfo->pos_x = new_x;
    }
}

void moveLeft() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    if (TetrisGameInfo->pos_x > 0 && \
         !isCollided(TetrisGameInfo->figure, \
         TetrisGameInfo->figure_h, TetrisGameInfo->figure_w, \
         TetrisGameInfo->pos_y, TetrisGameInfo->pos_x - 1)) {
        
        TetrisGameInfo->pos_x -= 1;
        printlog("x=%d y=%d x+w=%d y+h=%d", TetrisGameInfo->pos_x, TetrisGameInfo->pos_y,
        TetrisGameInfo->pos_x + TetrisGameInfo->figure_w,
        TetrisGameInfo->pos_y + TetrisGameInfo->figure_h);
    }
}

void moveRigth() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    if (TetrisGameInfo->pos_x + TetrisGameInfo->figure_w < FIELD_W && \
         !isCollided(TetrisGameInfo->figure, \
         TetrisGameInfo->figure_h, TetrisGameInfo->figure_w, \
         TetrisGameInfo->pos_y, TetrisGameInfo->pos_x + 1)) {

        TetrisGameInfo->pos_x += 1;
        printlog("x=%d y=%d x+w=%d y+h=%d", TetrisGameInfo->pos_x, TetrisGameInfo->pos_y,
        TetrisGameInfo->pos_x + TetrisGameInfo->figure_w,
        TetrisGameInfo->pos_y + TetrisGameInfo->figure_h);
    }
}

void dropDown() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    GameInfo_t gameInfo;
    
    int max_y = FIELD_H - TetrisGameInfo->figure_h + 1;
    for (int y = TetrisGameInfo->pos_y; y < max_y; y++) {
        if (isCollided(TetrisGameInfo->figure, \
            TetrisGameInfo->figure_h, TetrisGameInfo->figure_w, \
            y, TetrisGameInfo->pos_x)) {
            break;
        } else {
            TetrisGameInfo->pos_y = y;
            getch();
            gameInfo = updateCurrentState();
            renderGame(&gameInfo);
        }
    }
    setState(FSM_Attaching);
}

void attachFigure() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    for (int y = 0; y < TetrisGameInfo->figure_h; y++)
        for (int x = 0; x < TetrisGameInfo->figure_w; x++) 
            if (TetrisGameInfo->figure[y][x] != 0)
                TetrisGameInfo->field[y + TetrisGameInfo->pos_y][x + TetrisGameInfo->pos_x] \
                 = TetrisGameInfo->figure[y][x];
    
    deleteLines();
    if (currentState(FSM_Attaching))
        setState(FSM_Spawn);
    printlog("attachFigure() set FSM_Spawn");
}

void gamePause() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    if (TetrisGameInfo->pause == 1) {
        TetrisGameInfo->pause = 0;
        TetrisGameInfo->time = getTime();
        setState(FSM_Moving);
        printlog("Play game                ");
        timeout(GETCH_TIMEOUT);
    }
    else if (TetrisGameInfo->pause == 0) {
        TetrisGameInfo->pause = 1;
        setState(FSM_GamePause);
        printlog("Pause game                 ");
        timeout(-1);
    }
}

int makeChecksum(int num) {
    const int secret = SECRET_CHECKSUM;
    return (num ^ secret) + (num << 3);
}

void saveScore(int score) {
    score_t data = {0};
    data.score = score;
    data.checksum = make_checksum(score);
    FILE *f = fopen(BEST_SCORE_FILE, "wb");
    if (f) {
        fwrite(&data, sizeof(score_t), 1, f);
        fclose(f);
    }
}

int loadScore() {
    int res = 0;
    FILE *f = fopen(BEST_SCORE_FILE, "rb");
    if (f) {
        score_t data = {0};
        size_t n = fread(&data, sizeof(score_t), 1, f);
        fclose(f);
        if (data.checksum == make_checksum(data.score))
            res = data.score;
    }
    return res;
}