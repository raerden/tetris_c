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

bool isGameRunning() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    return !currentState(FSM_Terminate);
}

bool timerExpired() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    return (getTime() - TetrisGameInfo->time > TetrisGameInfo->speed);
}

//Игровой тик.
void stepGame() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();

    if (currentState(FSM_Spawn)) {
        spawnFigure();
    } else if (currentState(FSM_Moving)) {

        if (TetrisGameInfo->fastDrop || timerExpired()) {
            TetrisGameInfo->time = getTime();
            moveDown();
        } 

    } else if (currentState(FSM_Attaching)) {
        attachFigure();
    }
}

/*
перенести функции отсюда куда следует. и переименовать в brick_game.c
*/

int main() {
    winInit();
    printBoard();

    GameInfo_t gameInfo = {0};

    while(isGameRunning()) {
        userAction(); // функция frontend.c
        stepGame(); // функция backend.c
        gameInfo = updateCurrentState();// функция backend.c
        renderGame(&gameInfo); // функция frontend.c
    }

    terminateGame(&gameInfo);
    winClose();

    return 0;
}