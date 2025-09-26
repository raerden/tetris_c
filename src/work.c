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

bool processGame() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    return !currentState(FSM_Terminate);
}

void gameLoop() {
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
}

void stepGame() {
    //отслеживание shift по таймеру
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    long long time = getTime();
    if (currentState(FSM_Moving) &&
        time - TetrisGameInfo->time > TetrisGameInfo->speed) {
        TetrisGameInfo->time = time;
        setState(FSM_Shifting);
        printlog("updateCurrentState() set FSM_Shifting by timer");
    }
/*
 гпт предлагет перенести логиу смещения фигуры по таймер сюда
 убрать её из updateCurrentState
     if (fastDropActive) {
        // сдвигаем вниз на каждом кадре
        moveFigureDown();
        if (figureTouchedBottom()) {
            fastDropActive = false;
            attachFigure();
        }
    } else {
        // обычное падение с таймером/скоростью
        if (timerExpired()) {
            moveFigureDown();
        }
    }

*/
    if (currentState(FSM_Spawn)) {
        spawnFigure();
    } else if (currentState(FSM_Shifting)) {
        shiftFigure();
    } else if (currentState(FSM_Attaching)) {
        attachFigure();
    }
}
/*
переписать функцию movedown()
она будет вызываться если установлен флаг
bool fastDrop;
проверяет коллицизию и смещает фигуру на один вниз

updateCurrentState
при включенном флаге fastDrop дергает movedown()
тогда получится быстрое падение фигурки с отрисовкной накаждом кадре без вызова из бека

функция spawn устанавливает fastDrop = 0;

*/
int main() {
    winInit();
    printBoard();

    GameInfo_t gameInfo = {0};

    while(processGame()) {
        userAction(); // функция frontend.c
        stepGame(); // функция backend.c
        gameInfo = updateCurrentState();// функция backend.c
        renderGame(&gameInfo); // функция frontend.c
    }

    terminateGame(&gameInfo);
    winClose();

    return 0;
}