#include <stdio.h>
//#include <locale.h>
// #include "./gui/cli/defines.h"
#include "./gui/cli/frontend.h"
// #include "./brick_game/tetris/defines.h"
#include "./brick_game/tetris/backend.h"
#include "./brick_game.h"

// временно для отладки
void printlog2(const char *fmt, ...) {
    static int row = 25;  // начинаем печатать с 0-й строки
    if (row > 32) row = 25;
    va_list args;
    va_start(args, fmt);

    row++;
    move(row, 1);
    vw_printw(stdscr, fmt, args); // печать форматированной строки
    va_end(args);

    refresh(); // обновляем экран
}

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
        GameInfo.field = create_matrix(FIELD_H, FIELD_W);
    }

    // TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    // unsigned long long time = get_time();
    //  if (TetrisGameInfo->status == FSM_Moving &&
    //         time - TetrisGameInfo->time > TetrisGameInfo->speed) {
    //     TetrisGameInfo->status = FSM_Shifting;
    //     printlog("updateCurrentState() set FSM_Shifting by timer");
    //  }

    TetrisToGameInfo(&GameInfo);

    static int scr = 0;
    scr++;
    GameInfo.score = scr;

    return GameInfo;
}

void rotateFigure() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    clear_matrix(TetrisGameInfo->figure_tmp, FIGURE_FIELD_SIZE, FIGURE_FIELD_SIZE);
    //поворот направо
    //x идет в строку y
    //y идет в x, но с конца
    for (int y = 0; y < TetrisGameInfo->figure_h; y++) {
        for (int x = 0; x < TetrisGameInfo->figure_w; x++) {
            TetrisGameInfo->figure_tmp[x][TetrisGameInfo->figure_h - 1 - y] = TetrisGameInfo->figure[y][x];
        }
    }

    //проверить не перекрывает ли .figure_rotate не пустые клетки.
    // если нет. То копируем повернутую фигуру в .figure
    // передавать новый x с учетом коллизии об правую стенку при повороте
    // bool is_collided(int **figure, x, y);
    if (true) {
        int tmp = TetrisGameInfo->figure_w;
        TetrisGameInfo->figure_w = TetrisGameInfo->figure_h;
        TetrisGameInfo->figure_h = tmp;
        copy_matrix(TetrisGameInfo->figure, TetrisGameInfo->figure_tmp, FIGURE_FIELD_SIZE, FIGURE_FIELD_SIZE);
        if (TetrisGameInfo->pos_x + TetrisGameInfo->figure_w > FIELD_W) {
            TetrisGameInfo->pos_x = TetrisGameInfo->pos_x - (TetrisGameInfo->pos_x + TetrisGameInfo->figure_w - FIELD_W);
            // x +w =  11
            // field = 10
        }
    }
}

void moveLeft() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    if (TetrisGameInfo->pos_x > 0)
        TetrisGameInfo->pos_x -= 1;
    printlog("x=%d x+w=%d", TetrisGameInfo->pos_x, TetrisGameInfo->pos_x + TetrisGameInfo->figure_w);
}

void moveRigth() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    if (TetrisGameInfo->pos_x + TetrisGameInfo->figure_w < FIELD_W )
        TetrisGameInfo->pos_x += 1;
    printlog("x=%d x+w=%d", TetrisGameInfo->pos_x, TetrisGameInfo->pos_x + TetrisGameInfo->figure_w);
}

void startGame() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    TetrisGameInfo->time = get_time();
    TetrisGameInfo->speed = INITIAL_SPEED;
    // загрузить best score

    GenerateNextFigure();
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
        timeout(100);
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
    timeout(100);

    while(TetrisGameInfo->status != FSM_Terminate) {
        TetrisGameInfo = getTetrisGameInfo();
        printfsm();
        if (TetrisGameInfo->status == FSM_Start) {
            userAction();// чтение клавиатуры.
        } else if (TetrisGameInfo->status == FSM_Spawn) {
            // printlog("FSM_spawn. press key");
            // timeout(-1);
            // getch();
            // timeout(10);
            spawnFigure();
        } else if (TetrisGameInfo->status == FSM_Moving) {
            userAction();
        } else if (TetrisGameInfo->status == FSM_Shifting) {
            shiftFigure();
        } else if (TetrisGameInfo->status == FSM_Attaching) {
            //AttachFigure();
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

        // if (process_key(&action, &hold))//читаем клавиатуру
        //     userInput(action, hold);//если было нажатие, меняем FSM или вызываем соотв. функции

        // game = updateCurrentState();
        // print_stats(&game);
        // if (game.pause == 1) {
        //     print_pause(&game);
        // }
        // else {
        //     getTetrisGameInfo()->score += 1;
        //     print_field(&game);
        // }
    }

    terminate(&gameInfo);
    win_close();

    return 0;
}