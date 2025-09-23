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
    switch (TetrisGameInfo->status)
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

    // TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    // unsigned long long time = get_time();
    //  if (TetrisGameInfo->status == FSM_Moving &&
    //         time - TetrisGameInfo->time > TetrisGameInfo->speed) {
    //     TetrisGameInfo->status = FSM_Shifting;
    //     printlog("updateCurrentState() set FSM_Shifting by timer");
    //  }

    tetrisToGameInfo(&GameInfo);

    static int scr = 0;
    scr++;
    GameInfo.score = scr;

    return GameInfo;
}

bool is_collided(int **figure, int width, int heigth, int pos_x, int pos_y) {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    bool res = false;

    for (int y = 0; res == false && y < heigth; y++)
        for (int x = 0; res == false && x < width; x++) 
            if (TetrisGameInfo->figure[y][x] != 0 && TetrisGameInfo->field[y + pos_y][x + pos_x] != 0)
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

    if (!is_collided(TetrisGameInfo->figure_tmp, TetrisGameInfo->figure_h, TetrisGameInfo->figure_w, new_x, TetrisGameInfo->pos_y) && \
            TetrisGameInfo->pos_y + TetrisGameInfo->figure_w <= FIELD_H) {
        swapInt(&TetrisGameInfo->figure_w, &TetrisGameInfo->figure_h);
        copyMatrix(TetrisGameInfo->figure, TetrisGameInfo->figure_tmp, FIGURE_FIELD_SIZE, FIGURE_FIELD_SIZE);
        TetrisGameInfo->pos_x = new_x;
    }
}

void moveLeft() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    if (TetrisGameInfo->pos_x > 0 && \
         !is_collided(TetrisGameInfo->figure, \
         TetrisGameInfo->figure_w, TetrisGameInfo->figure_h, \
         TetrisGameInfo->pos_x - 1, TetrisGameInfo->pos_y)) {
        
        TetrisGameInfo->pos_x -= 1;
        printlog("x=%d y=%d x+w=%d y+h=%d", TetrisGameInfo->pos_x, TetrisGameInfo->pos_y,
        TetrisGameInfo->pos_x + TetrisGameInfo->figure_w,
        TetrisGameInfo->pos_y + TetrisGameInfo->figure_h);
    }
}

void moveRigth() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    if (TetrisGameInfo->pos_x + TetrisGameInfo->figure_w < FIELD_W && \
         !is_collided(TetrisGameInfo->figure, \
         TetrisGameInfo->figure_w, TetrisGameInfo->figure_h, \
         TetrisGameInfo->pos_x + 1, TetrisGameInfo->pos_y)) {

        TetrisGameInfo->pos_x += 1;
        printlog("x=%d y=%d x+w=%d y+h=%d", TetrisGameInfo->pos_x, TetrisGameInfo->pos_y,
        TetrisGameInfo->pos_x + TetrisGameInfo->figure_w,
        TetrisGameInfo->pos_y + TetrisGameInfo->figure_h);
    }
}

void moveDown() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    if (TetrisGameInfo->pos_y + TetrisGameInfo->figure_h < FIELD_H )
        TetrisGameInfo->pos_y += 1;

    printlog("x=%d y=%d x+w=%d y+h=%d", TetrisGameInfo->pos_x, TetrisGameInfo->pos_y,
        TetrisGameInfo->pos_x + TetrisGameInfo->figure_w,
        TetrisGameInfo->pos_y + TetrisGameInfo->figure_h);
}
void dropDown() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    GameInfo_t gameInfo;
    
    int max_y = FIELD_H - TetrisGameInfo->figure_h + 1;
    for (int y = TetrisGameInfo->pos_y; y < max_y; y++) {
        if (is_collided(TetrisGameInfo->figure, \
         TetrisGameInfo->figure_w, TetrisGameInfo->figure_h, \
         TetrisGameInfo->pos_x, y)) {
            break;
         } else {
            TetrisGameInfo->pos_y = y;
            getch();
            gameInfo = updateCurrentState();
            renderGame(&gameInfo);
         }
    }
    TetrisGameInfo->status = FSM_Attaching;
}

void attachFigure() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    for (int y = 0; y < TetrisGameInfo->figure_h; y++)
        for (int x = 0; x < TetrisGameInfo->figure_w; x++) 
            if (TetrisGameInfo->figure[y][x] != 0)
                TetrisGameInfo->field[y + TetrisGameInfo->pos_y][x + TetrisGameInfo->pos_x] \
                 = TetrisGameInfo->figure[y][x];
    TetrisGameInfo->status = FSM_Spawn;
    printlog("attachFigure() set FSM_Spawn");
}

void startGame() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    TetrisGameInfo->time = get_time();
    TetrisGameInfo->speed = INITIAL_SPEED;
    // загрузить best score

    genNextFigure();
    spawnFigure();
    TetrisGameInfo->status = FSM_Moving;
    printlog("startGame() set to FSM_Moving             ");
}

void gamePause() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    if (TetrisGameInfo->pause == 1) {
        TetrisGameInfo->pause = 0;
        TetrisGameInfo->time = get_time();
        TetrisGameInfo->status = FSM_Moving;
        printlog("Play game                ");
        timeout(GETCH_WAIT);
    }
    else if (TetrisGameInfo->pause == 0) {
        TetrisGameInfo->pause = 1;
        TetrisGameInfo->status = FSM_GamePause;
        printlog("Pause game                 ");
        timeout(-1);
    }
}

void shiftFigure() {
    static int cnt = 0;

    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    if (TetrisGameInfo->pos_y < FIELD_H - 5) {
        TetrisGameInfo->pos_y += 1;
        TetrisGameInfo->time = get_time();
        TetrisGameInfo->status = FSM_Moving;
        printlog("shiftFigure() set to FSM_Moving");
    } else {
        TetrisGameInfo->status = FSM_Spawn;
        //printlog("shiftFigure() set to FSM_Spawn %d", ++cnt);
    }
    
}

// Каждая библиотека с игрой должна иметь функцию, принимающую на вход пользовательский ввод.
void userInput(UserAction_t action, bool hold) {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    // printlog("userinput() action=%d", action);
    
    if (action == Start && TetrisGameInfo->status == FSM_Start) {
        startGame();
    } else if (action == Pause && TetrisGameInfo->status != FSM_Start) {
        gamePause();
    } else if (action == Action && TetrisGameInfo->status == FSM_Moving) {
        rotateFigure();
    } else if (action == Left && TetrisGameInfo->status == FSM_Moving) {
        moveLeft();
    } else if (action == Right && TetrisGameInfo->status == FSM_Moving) {
        moveRigth();
    } else if (action == Up && TetrisGameInfo->status == FSM_Moving) {
        spawnFigure();
    } else if (action == Down && TetrisGameInfo->status == FSM_Moving) {
        // moveDown();
        dropDown();
    } else if (action == Terminate) {
        TetrisGameInfo->status = FSM_Terminate;
    }

}

void userAction() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();

    UserAction_t action = 0;
    bool hold = false;
    if (process_key(&action, &hold)) // Функция frontend.c
        userInput(action, hold);
}


int main() {
    
    win_init(-1);

    srand(get_time()); //сброс rand() текущим временем

    print_board();

    GameInfo_t gameInfo = {0};
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();

    printlog("Initial. fsm=%d", TetrisGameInfo->status);
    timeout(GETCH_WAIT);

    while(TetrisGameInfo->status != FSM_Terminate) {
        TetrisGameInfo = getTetrisGameInfo();
        printfsm();
        if (TetrisGameInfo->status == FSM_Start) {
            userAction();// чтение клавиатуры.
        } else if (TetrisGameInfo->status == FSM_Spawn) {
            spawnFigure();
        } else if (TetrisGameInfo->status == FSM_Moving) {
            userAction();
        } else if (TetrisGameInfo->status == FSM_Shifting) {
            shiftFigure();
        } else if (TetrisGameInfo->status == FSM_Attaching) {
            attachFigure();
        } else if (TetrisGameInfo->status == FSM_GamePause) {
            userAction();
        } else if (TetrisGameInfo->status == FSM_GameWin) {
            // GameWin();
        } else if (TetrisGameInfo->status == FSM_GameOver) {
            // GameOver();
        }
        
        
        if (TetrisGameInfo->status != FSM_Start) {
            gameInfo = updateCurrentState();
            renderGame(&gameInfo);
        }
    }

    terminate(&gameInfo);
    win_close();

    return 0;
}