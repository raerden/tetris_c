#include "backend.h"

void setScore() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    TetrisGameInfo->score += 600;
    TetrisGameInfo->high_score = TetrisGameInfo->score;
}

// Каждая библиотека с игрой должна иметь функцию, принимающую на вход пользовательский ввод.
// дергается из фронта если нажали кнопку.
void userInput(UserAction_t action, bool hold) {
    if (action == Terminate) {
        terminateGame();
    } else if (action == Start) {
        startGame();
    } else if (action == Pause && !currentState(FSM_Start)) {
        gamePause();
    } else if (currentState(FSM_Moving)) {
        switch (action)
        {
            case Action: rotateFigure(); break;
            case Left: moveLeft(); break;
            case Right: moveRigth(); break;
            case Down: dropDown(); break;
            // case Up: spawnFigure(); break;
            break;
        }
    }
}

//вызывается в цикле while frontend пока идет игра
GameInfo_t updateCurrentState() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();

    if (currentState(FSM_Spawn)) {
        spawnFigure();
    } else if (currentState(FSM_Moving)) {

        if (TetrisGameInfo->fastDrop || isTimerExpired()) {
            moveDown();
        } 

    } else if (currentState(FSM_Attaching)) {
        attachFigure();
    }
    
    return tetrisToGameInfo();
}

//Хранит внутри static структуру с информацией об игре
TetrisGameInfo_t *getTetrisGameInfo() {
    static TetrisGameInfo_t TetrisGameInfo = {0};
    if (TetrisGameInfo.field == NULL) {
        //первый вызов функции. Инициализация
        srand(getTime()); //сброс rand() текущим временем
        TetrisGameInfo.field = createMatrix(FIELD_H, FIELD_W);
        TetrisGameInfo.field_front = createMatrix(FIELD_H, FIELD_W);
        TetrisGameInfo.next = createMatrix(FIGURE_FIELD_SIZE, FIGURE_FIELD_SIZE);
        TetrisGameInfo.figure = createMatrix(FIGURE_FIELD_SIZE, FIGURE_FIELD_SIZE);
        TetrisGameInfo.figure_tmp = createMatrix(FIGURE_FIELD_SIZE, FIGURE_FIELD_SIZE);
        tetrisLogo();
        TetrisGameInfo.state = FSM_Start;
    }
    return &TetrisGameInfo;
}

GameInfo_t tetrisToGameInfo() {
    const TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    copyMatrix(TetrisGameInfo->field_front, TetrisGameInfo->field, FIELD_H, FIELD_W);

    for (int y = 0; y < TetrisGameInfo->figure_h; y++)
        for (int x = 0; x < TetrisGameInfo->figure_w; x++) {
            int field_x = x + TetrisGameInfo->pos_x;
            int field_y = y + TetrisGameInfo->pos_y;
            if (TetrisGameInfo->figure[y][x] > 0 && field_x < FIELD_W && field_y < FIELD_H)
                TetrisGameInfo->field_front[field_y][field_x] = TetrisGameInfo->figure[y][x];
        }

    GameInfo_t gameInfo = {0};
    gameInfo.field = TetrisGameInfo->field_front;
    gameInfo.next = TetrisGameInfo->next;
    gameInfo.score = TetrisGameInfo->score;
    gameInfo.high_score = TetrisGameInfo->high_score;
    gameInfo.level = TetrisGameInfo->level;
    gameInfo.speed = TetrisGameInfo->speed;
    gameInfo.pause = TetrisGameInfo->pause;
    return gameInfo;
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

void startGame() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();

    TetrisGameInfo->score = 0;
    TetrisGameInfo->high_score = loadScore();
    TetrisGameInfo->level = 1;
    TetrisGameInfo->speed = calcSpeed(TetrisGameInfo->level);
    TetrisGameInfo->pause = 0;
    TetrisGameInfo->fastDrop = false;

    clearMatrix(TetrisGameInfo->field, FIELD_H, FIELD_W);
    genNextFigure();
    spawnFigure();
}

void gamePause() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    if (TetrisGameInfo->pause == 1) {
        TetrisGameInfo->pause = 0;
        TetrisGameInfo->time = getTime();
        setState(FSM_Moving);
    }
    else if (TetrisGameInfo->pause == 0) {
        TetrisGameInfo->pause = 1;
        setState(FSM_GamePause);
    }
}

void gameWin() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    saveScore(TetrisGameInfo->high_score);
    TetrisGameInfo->pause = 2;
    setState(FSM_Start);
}

void gameOver() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    saveScore(TetrisGameInfo->high_score);
    TetrisGameInfo->pause = 3;
    setState(FSM_Start);
}

int calcSpeed(int level) {
    //вычисление скорости по формуле (1250 * (0,8 ^ level))
    double degree = 1;
    for (int i = 0; i < level; i++)
        degree *= 0.8;
    return (int)INITIAL_SPEED * degree;
}

bool isFullRow(int **field, int row) {
    bool full = true;
    for (int col = 0; full && col < FIELD_W; col++)
        full = field[row][col] != 0;
    return full;
}

void removeRow(int **field, int row) {
    int *tmp = field[row];
    for (int r = row; r > 0; r--)
        field[r] = field[r - 1];
    field[0] = tmp;
    for (int col = 0; col < FIELD_W; col++)
        field[0][col] = 0;
}

int calcScore(int full_rows) {
    int score = 0;
    switch (full_rows)
    {
        case 1: score = 100; break;
        case 2: score = 300; break;
        case 3: score = 700; break;
        case 4: score = 1500; break;
    }
    return score;
}

void deleteLines() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();

    int full_rows = 0;
    for (int row = FIELD_H - 1; row > 0; row--)
        while(isFullRow(TetrisGameInfo->field, row)) {
            removeRow(TetrisGameInfo->field, row);
            full_rows++;
        }

    if (full_rows > 0) {
        TetrisGameInfo->score += calcScore(full_rows);

        if (TetrisGameInfo->score > TetrisGameInfo->high_score)
            TetrisGameInfo->high_score = TetrisGameInfo->score;
        
        int level = TetrisGameInfo->score / LEVEL_TRESHOLD + 1;
        if (level > 10) {
            gameWin();
        } else if (level > TetrisGameInfo->level) {
            TetrisGameInfo->level = level;
            TetrisGameInfo->speed = calcSpeed(level);
        }
    }
}


static long long getTime() {
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

void terminateGame() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    // saveScore(TetrisGameInfo->high_score);
    freeMatrix(TetrisGameInfo->field, FIELD_H);
    freeMatrix(TetrisGameInfo->next, FIGURE_FIELD_SIZE);
    freeMatrix(TetrisGameInfo->field_front, FIELD_H);
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
    int spawn_x = SPAWN_X - (TetrisGameInfo->next_w == 4 ? 1 : 0);
    // проверить коллизию из фигуры. Поражение если стакан заполнен
    if (isCollided(TetrisGameInfo->next, TetrisGameInfo->next_h, TetrisGameInfo->next_w, SPAWN_Y, spawn_x)) {
        gameOver();
    } else {
        copyMatrix(TetrisGameInfo->figure, TetrisGameInfo->next, FIGURE_FIELD_SIZE, FIGURE_FIELD_SIZE);
        TetrisGameInfo->figure_w = TetrisGameInfo->next_w;
        TetrisGameInfo->figure_h = TetrisGameInfo->next_h;
        TetrisGameInfo->pos_x = spawn_x;
        TetrisGameInfo->pos_y = SPAWN_Y;
        TetrisGameInfo->fastDrop = false;
        genNextFigure();
        TetrisGameInfo->time = getTime();
        setState(FSM_Moving);
    }
}

bool isCollided(int **figure, int heigth, int width, int pos_y, int pos_x) {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    bool res = false;

    //коллизии с краями игрового поля
    if (pos_x < 0 || pos_x + width > FIELD_W || heigth + pos_y > FIELD_H) {
        res = true;
    }
  
    //коллизии с фигурами на поле
    for (int y = 0; res == false && y < heigth; y++)
        for (int x = 0; res == false && x < width; x++) 
            if (figure[y][x] > 0 && TetrisGameInfo->field[y + pos_y][x + pos_x] > 0) {
                res = true;
            }
    
    return res;
}

bool isTimerExpired() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    return getTime() - TetrisGameInfo->time > TetrisGameInfo->speed;
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

    //Коллизия при повороте с правой стенкой. Отскок фигуры влево.
    int new_x = TetrisGameInfo->pos_x;
    if (TetrisGameInfo->pos_x + TetrisGameInfo->figure_h > FIELD_W)
        new_x = TetrisGameInfo->pos_x - (TetrisGameInfo->pos_x + TetrisGameInfo->figure_h - FIELD_W);

    if (!isCollided(TetrisGameInfo->figure_tmp, TetrisGameInfo->figure_w, TetrisGameInfo->figure_h, TetrisGameInfo->pos_y, new_x)) {
        swapInt(&TetrisGameInfo->figure_w, &TetrisGameInfo->figure_h);
        copyMatrix(TetrisGameInfo->figure, TetrisGameInfo->figure_tmp, FIGURE_FIELD_SIZE, FIGURE_FIELD_SIZE);
        TetrisGameInfo->pos_x = new_x;
    }
}

void moveLeft() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    if (!isCollided(TetrisGameInfo->figure, \
         TetrisGameInfo->figure_h, TetrisGameInfo->figure_w, \
         TetrisGameInfo->pos_y, TetrisGameInfo->pos_x - 1)) {
        
        TetrisGameInfo->pos_x -= 1;
    }
}

void moveRigth() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    if (!isCollided(TetrisGameInfo->figure, \
         TetrisGameInfo->figure_h, TetrisGameInfo->figure_w, \
         TetrisGameInfo->pos_y, TetrisGameInfo->pos_x + 1)) {

        TetrisGameInfo->pos_x += 1;
    }
}

void moveDown() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    if (isCollided(TetrisGameInfo->figure, \
        TetrisGameInfo->figure_h, TetrisGameInfo->figure_w, \
        TetrisGameInfo->pos_y + 1, TetrisGameInfo->pos_x)) {
        setState(FSM_Attaching);
    } else {
        TetrisGameInfo->pos_y++;
        if (!TetrisGameInfo->fastDrop)
            setState(FSM_Moving);
        TetrisGameInfo->time = getTime();
    }
}

void dropDown() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    TetrisGameInfo->fastDrop = true;
}

void attachFigure() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    for (int y = 0; y < TetrisGameInfo->figure_h; y++)
        for (int x = 0; x < TetrisGameInfo->figure_w; x++) 
            if (TetrisGameInfo->figure[y][x] != 0)
                TetrisGameInfo->field[y + TetrisGameInfo->pos_y][x + TetrisGameInfo->pos_x] \
                 = TetrisGameInfo->figure[y][x];
    
    deleteLines();//Может выставить FSM_Start если прошли 10 уровней = Победа.
    if (currentState(FSM_Attaching))
        setState(FSM_Spawn);
}

int genCheckSum(int num) {
    const int secret = SECRET_CHECKSUM;
    return (num ^ secret) + (num << 3);
}

void saveScore(int score) {
    score_t data = {0};
    data.score = score;
    data.checksum = genCheckSum(score);
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
        if (data.checksum == genCheckSum(data.score))
            res = data.score;
    }
    return res;
}

void tetrisLogo() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();

    // данные заставки "TETRIS"
    int logo[FIELD_H][FIELD_W] = {
        {3,3,1,0,0,0,0,0,0,0},
        {0,3,0,0,3,3,3,1,0,0},
        {0,3,0,0,3,0,0,0,0,0},
        {0,1,0,0,1,1,1,0,0,0},
        {0,0,0,0,2,0,0,0,0,0},
        {0,0,0,0,2,2,2,1,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,1,2,2,0,3,3,3,0,0},
        {0,0,2,0,0,3,0,0,1,0},
        {0,0,2,0,0,1,0,1,0,0},
        {0,0,1,0,0,2,2,0,0,0},
        {0,0,0,0,0,2,0,1,0,0},
        {0,0,0,0,0,2,0,0,1,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,1,0,0,0,1,1,0},
        {0,0,0,2,0,0,1,0,0,1},
        {0,0,0,3,0,0,0,1,0,0},
        {0,0,0,4,0,0,0,0,1,0},
        {0,0,0,5,0,0,7,0,0,1},
        {0,0,0,0,0,0,0,1,1,0}
    };

    // копируем в поле
    for (int y = 0; y < FIELD_H; y++) {
        for (int x = 0; x < FIELD_W; x++) {
            TetrisGameInfo->field[y][x] = logo[y][x];
        }
    }
}