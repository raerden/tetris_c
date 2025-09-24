#include <stdio.h>
//#include <locale.h>
// #include "./gui/cli/defines.h"
#include "./gui/cli/frontend.h"
// #include "./brick_game/tetris/defines.h"
#include "./brick_game/tetris/backend.h"
#include "./brick_game.h"

void printlog(const char *fmt, ...) {
    static WINDOW *logwin = NULL;
    static int height = 8;   // сколько строк для лога
    static int width  = 80;  // ширина окна
    static int starty = 25;  // начальная позиция по Y
    static int startx = 1;   // начальная позиция по X

    if (logwin == NULL) {
        logwin = newwin(height, width, starty, startx);
        scrollok(logwin, TRUE);   // разрешаем прокрутку
    }

    va_list args;
    va_start(args, fmt);
    vw_printw(logwin, fmt, args);
    va_end(args);

    wprintw(logwin, "\n"); // перевод строки
    wrefresh(logwin);
}

void printfsm() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    switch (TetrisGameInfo->state)
    {
    // case 0: printlog("FSM_Start         "); break;
    // case 1: printlog("FSM_Spawn         "); break;
    // case 2: printlog("FSM_Moving        "); break;
    // case 3: printlog("FSM_Shifting      "); break;
    // case 4: printlog("FSM_Attaching     "); break;
    // case 5: printlog("FSM_GamePause     "); break;
    // case 6: printlog("FSM_GameWin       "); break;
    // case 7: printlog("FSM_GameOver      "); break;
    // case 8: printlog("FSM_Terminate     "); break;
    
    default:
        break;
    }
}


// функция brick_game.c
GameInfo_t updateCurrentState() {
    static GameInfo_t GameInfo = {0};
    if (GameInfo.field == NULL) {
        GameInfo.field = createMatrix(FIELD_H, FIELD_W);
    }

    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    long long time = getTime();
    if (currentState(FSM_Moving) &&
        time - TetrisGameInfo->time > TetrisGameInfo->speed) {
        TetrisGameInfo->time = time;
        setState(FSM_Shifting);
        printlog("updateCurrentState() set FSM_Shifting by timer");
    }

    tetrisToGameInfo(&GameInfo);

    //счетчик вызова updateCurrentState для визуализации тиков
    // static int scr = 0;
    // GameInfo.score = scr++;

    return GameInfo;
}

// функция brick_game.c
void renderGame(GameInfo_t *GameInfo) {
    if (GameInfo->pause == 0) {
        printStats(GameInfo);
        printNext(GameInfo);
        printField(GameInfo);
    }
    else 
        printPause(GameInfo);
}



void moveDown() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    if (TetrisGameInfo->pos_y + TetrisGameInfo->figure_h < FIELD_H )
        TetrisGameInfo->pos_y += 1;

    printlog("x=%d y=%d x+w=%d y+h=%d", TetrisGameInfo->pos_x, TetrisGameInfo->pos_y,
        TetrisGameInfo->pos_x + TetrisGameInfo->figure_w,
        TetrisGameInfo->pos_y + TetrisGameInfo->figure_h);
}

void gameWin() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    TetrisGameInfo->pause = 2;
    setState(FSM_Start);
}

void gameOver() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
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

void saveBestScore() {

}

int loadBestScore() {
    return 0;
}

void startGame() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();

    TetrisGameInfo->score = 0;
    TetrisGameInfo->high_score = loadBestScore();
    TetrisGameInfo->level = 1;
    TetrisGameInfo->speed = calcSpeed(TetrisGameInfo->level);
    TetrisGameInfo->pause = 0;

    clearMatrix(TetrisGameInfo->field, FIELD_H, FIELD_W);
    genNextFigure();
    spawnFigure();
    setState(FSM_Moving);
    printlog("startGame() set to FSM_Moving             ");
}

void shiftFigure() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    int max_y = FIELD_H - TetrisGameInfo->figure_h;
    if (TetrisGameInfo->pos_y == max_y || isCollided(TetrisGameInfo->figure, \
        TetrisGameInfo->figure_h, TetrisGameInfo->figure_w, \
        TetrisGameInfo->pos_y + 1, TetrisGameInfo->pos_x)) {
            setState(FSM_Attaching);
        } else {
            TetrisGameInfo->pos_y += 1;
            setState(FSM_Moving);
        }
}

// Каждая библиотека с игрой должна иметь функцию, принимающую на вход пользовательский ввод.
void userInput(UserAction_t action, bool hold) {
    if (action == Terminate) {
        setState(FSM_Terminate);
    } else if (action == Start && currentState(FSM_Start)) {
        startGame();
    } else if (action == Pause && !currentState(FSM_Start)) {
        gamePause();
    } else if (currentState(FSM_Moving)) {
        switch (action)
        {
            case Action: rotateFigure(); break;
            case Left: moveLeft(); break;
            case Right: moveRigth(); break;
            case Up: gameWin(); break;
            // case Up: spawnFigure(); break;
            case Down: dropDown(); break;
        }
    }
}

void userAction() {
    UserAction_t action = 0;
    bool hold = false;
    if (processKey(&action, &hold)) // Функция frontend.c
        userInput(action, hold);
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

int main() {
    
    winInit(-1);

    srand(getTime()); //сброс rand() текущим временем

    printBoard();

    GameInfo_t gameInfo = {0};
    
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    TetrisGameInfo->pause = 4;// Приглашение к игре.

    printlog("Initial. fsm=%d", TetrisGameInfo->state);
    timeout(GETCH_TIMEOUT);

    while(!currentState(FSM_Terminate)) {
        if (currentState(FSM_Start)) {
            userAction();// чтение клавиатуры.
        } else if (currentState(FSM_Spawn)) {
            spawnFigure();
        } else if (currentState(FSM_Moving)) {
            userAction();
        } else if (currentState(FSM_Shifting)) {
            shiftFigure();
        } else if (currentState(FSM_Attaching)) {
            attachFigure();
        } else if (currentState(FSM_GamePause)) {
            userAction();
        }

        gameInfo = updateCurrentState();
        renderGame(&gameInfo);
    }

    terminate(&gameInfo);
    winClose();

    return 0;
}