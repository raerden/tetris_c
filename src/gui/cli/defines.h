#ifndef DEFINES_H
#define DEFINES_H

#define BOARDS_BEGIN 2

#define MVPRINTW(y, x, ...) mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__)
#define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c)


#define BOARD_H     20
#define BOARD_W     20
#define INFO_W      13

#define KEY_ACTION 32 // пробел
#define KEY_TERMINATE 27 // ESC
#define KEY_PAUSE_UPPERCASE 80 // P
#define KEY_PAUSE 112 // p
#define KEY_START_N 10 // \n Enter
#define KEY_START_R 13 // \r  Enter

#define HOLD_THRESHOLD_MS   100  // через сколько после первого нажатия считаем «зажато»

#define COLR_RED 1
#define COLR_ORANGE 208
#define COLR_YELLOW 11
#define COLR_PINK   213
#define COLR_GREEN  22
#define COLR_BLUE   20
#define COLR_PURPLE 90

#define GETCH_TIMEOUT 10
#define ESC_TIMEOUT 30
#define PRESS_ENTER_MESSAGE "Press ENTER to start"
#define PRESS_ENTER_MESSAGE_LEN 20
#define PRESS_P_MESSAGE "Press P to continue"
#define PRESS_P_MESSAGE_LEN 19
#define PRESS_Q_MESSAGE "Press ESC to quit"
#define PRESS_Q_MESSAGE_LEN 17

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