#ifndef DEFINES_H
#define DEFINES_H

//#define GET_USER_INPUT getch()

#define BOARDS_BEGIN 2

#define MVPRINTW(y, x, ...) mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__)
#define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c)
//#define CLEAR_BACKPOS(y, x) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), ' ')

#define YOU_WON "You Won!"
#define YOU_LOSE "Game Over"
#define INTRO_MESSAGE "Press ENTER to start"
#define INTRO_MESSAGE_LEN     21


#define INITIAL_TIMEOUT  600
#define FIELD_W     10
#define FIELD_H     20

#define BOARD_H     20
#define BOARD_W     20

#define INFO_W   12

#define BANNER_N    10
#define BANNER_M    100

#define SUCCESS   0
#define ERROR     1

#define NO_INPUT -1

#define ESCAPE 27
#define ENTER_KEY 10

#define COLR_RED 1
#define COLR_ORANGE 208
#define COLR_YELLOW 11
#define COLR_PINK   213
#define COLR_GREEN  22
#define COLR_BLUE   20
#define COLR_PURPLE 90

#endif