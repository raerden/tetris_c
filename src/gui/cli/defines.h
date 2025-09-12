#ifndef DEFINES_H
#define DEFINES_H

#define BOARDS_BEGIN 2

#define MVPRINTW(y, x, ...) mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__)
#define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c)
//#define CLEAR_BACKPOS(y, x) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), ' ')

#define FIELD_W     10
#define FIELD_H     20

#define BOARD_H     20
#define BOARD_W     20

#define INFO_W   12

// #define SUCCESS   0
// #define ERROR     1

// #define INITIAL_TIMEOUT  600
// #define NO_INPUT -1

// #define ESCAPE 27
// #define ENTER_KEY 10

#define COLR_RED 1
#define COLR_ORANGE 208
#define COLR_YELLOW 11
#define COLR_PINK   213
#define COLR_GREEN  22
#define COLR_BLUE   20
#define COLR_PURPLE 90

#define PRESS_ENTER_MESSAGE "Press ENTER to start"
#define PRESS_ENTER_MESSAGE_LEN 20
#define PRESS_P_MESSAGE "Press P to continue"
#define PRESS_P_MESSAGE_LEN 19
#define PRESS_Q_MESSAGE "Press Q to quit"
#define PRESS_Q_MESSAGE_LEN 15

#define PAUSE_MESSAGE "Game on pause"
#define PAUSE_MESSAGE_LEN 13
#define WIN_MESSAGE "!! YOU WON !!"
#define WIN_MESSAGE_LEN 13
#define LOSE_MESSAGE "GAME OVER"
#define LOSE_MESSAGE_LEN 9

#define SMILE_PAUSE "(O_~)"
#define SMILE_WIN "(^_^)"
#define SMILE_SAD "(~_~)"
#define SMILE_WIDTH 5

#endif