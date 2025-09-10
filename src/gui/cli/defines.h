#ifndef DEFINES_H
#define DEFINES_H

#define WIN_INIT(time) {\
                             initscr();\
                             noecho();\
                             curs_set(0);\
                             keypad(stdscr, TRUE);\
                             start_color();\
                             init_pair(1, COLOR_BLACK, COLOR_RED);\
                             timeout(time);\
                        }

#define GET_USER_INPUT getch()

#define PRINT_FROG(x, y) mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), "@")
#define MVPRINTW(y, x, ...) mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__)
#define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c)
#define CLEAR_BACKPOS(y, x) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), ' ')

#define YOU_WON "tests/game_progress/you_won.txt"
#define YOU_LOSE "tests/game_progress/you_lose.txt"
#define INTRO_MESSAGE "Press ENTER to start"
#define INTRO_MESSAGE_LEN     21

#define MAX_WIN_COUNT    10

// #define ROWS_MAP 14
// #define COLS_MAP 90

#define BOARDS_BEGIN 2

#define INITIAL_TIMEOUT  600

#define FIELD_W     10
#define BOARD_H     20
#define BOARD_W     20

#define INFO_W   12
// #define MAP_PADDING 3

#define BANNER_N    10
#define BANNER_M    100

#define SUCCESS   0
#define ERROR     1

#define NO_INPUT -1

#define ESCAPE 27
#define ENTER_KEY 10

#endif