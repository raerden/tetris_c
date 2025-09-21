#include <stdio.h>
//#include <locale.h>
// #include "./gui/cli/defines.h"
#include "./gui/cli/frontend.h"
// #include "./brick_game/tetris/defines.h"
#include "./brick_game/tetris/backend.h"
#include "./brick_game.h"

// временно для отладки
void printlog(const char *fmt, ...) {
    static int row = 25;  // начинаем печатать с 0-й строки
    if (row > 32) row = 25;
    va_list args;
    va_start(args, fmt);

    // печатаем строку с аргументами, как в printf
    // mvprintw(row++, 1, "");       // перемещаем курсор
    move(row++, 1); 
    vw_printw(stdscr, fmt, args); // печать форматированной строки
    va_end(args);

    refresh(); // обновляем экран
}
//mvprintw(25+y, 1, "x(col)=%d, y(row)=%d, field_x(col)=%d, field_y(row)=%d", x, y, field_x, field_y);

// функция brick_game.c
GameInfo_t updateCurrentState() {
    static GameInfo_t GameInfo = {0};
    if (GameInfo.field == NULL) {
        GameInfo.field = create_matrix(FIELD_H, FIELD_W);
    }

    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    unsigned long long time = get_time();
    
     if (TetrisGameInfo->status = FSM_Moving && time - TetrisGameInfo->time > TetrisGameInfo->speed) {
        // printlog("speed= %d", TetrisGameInfo->speed);
        TetrisGameInfo->status = FSM_Shifting;
     }

    TetrisToGameInfo(&GameInfo);

    static int scr = 0;
    scr++;
    GameInfo.score = scr;

    return GameInfo;
}

void startGame() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    TetrisGameInfo->time = get_time();
    TetrisGameInfo->speed = INITIAL_SPEED;
    // загрузить best score

    GenerateNextFigure();
    spawnFigure();
    TetrisGameInfo->status = FSM_Moving;
    printlog("startGame() set to FSM_Moving");
}

void gamePause() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    if (TetrisGameInfo->pause == 1) {
        TetrisGameInfo->pause = 0;
        TetrisGameInfo->time = get_time();
        printlog("pause=0");
        timeout(10);
    }
    else if (TetrisGameInfo->pause == 0) {
        TetrisGameInfo->pause = 1;
        printlog("pause=1");
        timeout(-1);
    }
}

void shiftFigure() {
    static int cnt = 0;

    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    if (TetrisGameInfo->pos_y < FIELD_H - 5) {
        TetrisGameInfo->pos_y += 1;
        TetrisGameInfo->status = FSM_Moving;
        TetrisGameInfo->time = get_time();
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
    } else if (action == Left && TetrisGameInfo->status == FSM_Moving) {
        // shiftFigure();
        TetrisGameInfo->status = FSM_Shifting;
        printlog("userInput() set to FSM_Shifting");

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
    timeout(10);

    while(TetrisGameInfo->status != FSM_Terminate) {
        TetrisGameInfo = getTetrisGameInfo();
        printlog("FSM_%d", TetrisGameInfo->status);
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
        // } else if (TetrisGameInfo->status == FSM_GamePause) {
        //     gamePause();
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