#include "frontend.h"


void winInit(int time) {
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(time);
    set_escdelay(30);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_BLACK, COLR_RED);    // палка     красная
        init_pair(2, COLOR_BLACK, COLR_ORANGE); // Г-правая   оранжевая
        init_pair(3, COLOR_BLACK, COLR_YELLOW); // Г-левая  желтая
        init_pair(4, COLOR_BLACK, COLR_PINK);   // квадрат   розовый
        init_pair(5, COLOR_BLACK, COLR_GREEN);  // z-правая  зеленая
        init_pair(6, COLOR_BLACK, COLR_BLUE);   // T         синяя
        init_pair(7, COLOR_BLACK, COLR_PURPLE); // z-левая   пурпурная
    }
}

void winClose() {
    reset_shell_mode();
    echo();
    nocbreak();
    keypad(stdscr, FALSE);
    endwin();
}

void printBoard() {
    // игровое поле
    printRectangle(0, BOARD_H + 1, 0, BOARD_W + 1);
    // поле статистики
    printRectangle(0, BOARD_H + 1, BOARD_W + 2, BOARD_W + INFO_W + 3);
   
    printRectangle(1, 3, BOARD_W + 3, BOARD_W + INFO_W + 2);
    MVPRINTW(2, BOARD_W + 4, "BEST");

    printRectangle(4, 6, BOARD_W + 3, BOARD_W + INFO_W + 2);
    MVPRINTW(5, BOARD_W + 4, "SCORE");

    printRectangle(7, 9, BOARD_W + 3, BOARD_W + INFO_W + 2);
    MVPRINTW(8, BOARD_W + 4, "LEVEL");

    MVPRINTW(10, BOARD_W + 7, "NEXT");
}

void printRectangle(int top_y, int bottom_y, int left_x, int right_x)
{
    MVADDCH(top_y, left_x, ACS_ULCORNER);

    int i = left_x + 1;

    for (;i < right_x; i++)
        MVADDCH(top_y, i, ACS_HLINE);
    MVADDCH(top_y, i, ACS_URCORNER);

    for (int i = top_y + 1; i < bottom_y; i++)
    {
        MVADDCH(i, left_x, ACS_VLINE);
        MVADDCH(i, right_x, ACS_VLINE);
    }

    MVADDCH(bottom_y, left_x, ACS_LLCORNER);
    i = left_x + 1;
    for (;i < right_x; i++)
        MVADDCH(bottom_y, i, ACS_HLINE);
    MVADDCH(bottom_y, i, ACS_LRCORNER);
}

void printNext(GameInfo_t *GameInfo) {
    // очистка предыдущей фигуры
    for (int i = 0; i < NEXT_SIZE; i++)
        for (int j = 0; j < NEXT_SIZE; j++) {
            MVADDCH(12 + i, BOARD_W + 4 + (j * 2 + 1), ' ');
            MVADDCH(12 + i, BOARD_W + 4 + (j * 2 + 2), ' ');
        }

    int color = 0;
    bool color_on = has_colors();
    char ch_L = color_on ? ' ' : '[';
    char ch_R = color_on ? ' ' : ']';

    // найти цвет фигурки в матрице **next
    for (int i = 0; color_on && i < NEXT_SIZE; i++) 
        if (GameInfo->next[0][i] > 0) {
            color = GameInfo->next[0][i];
            break;
        }

    if (color_on) attron(COLOR_PAIR(color));
    for (int i = 0; i < NEXT_SIZE; i++)
        for (int j = 0; j < NEXT_SIZE; j++) 
            if (GameInfo->next[i][j]) {
                MVADDCH(12 + i, BOARD_W + 4 + (j * 2 + 1), ch_L);
                MVADDCH(12 + i, BOARD_W + 4 + (j * 2 + 2), ch_R);
        }
    if (color_on) attroff(COLOR_PAIR(color));
}

void printStats(GameInfo_t *GameInfo) {
    MVPRINTW(2, BOARD_W + 10, "    ");
    MVPRINTW(2, BOARD_W + 10, "%-d", GameInfo->high_score);

    MVPRINTW(5, BOARD_W + 10, "    ");
    MVPRINTW(5, BOARD_W + 10, "%-d", GameInfo->score);

    MVPRINTW(8, BOARD_W + 10, "  ");
    MVPRINTW(8, BOARD_W + 10, "%-d", GameInfo->level);

    printNext(GameInfo);
}

void clearField() {
    for (int i = 0; i < FIELD_H; i++)
        for (int j = 0; j < FIELD_W; j++) {
            MVADDCH(i+1, (j * 2 + 1), ' ');
            MVADDCH(i+1, (j * 2 + 2), ' ');
        }
}

void printField(GameInfo_t *GameInfo) {
    bool color_on = has_colors();
    char ch_L = color_on ? ' ' : ' ';
    char ch_R = color_on ? ' ' : ' ';
    for (int i = 0; i < BOARD_H; i++)
        for (int j = 0; j < FIELD_W; j++) {
            if (color_on) attron(COLOR_PAIR(GameInfo->field[i][j]));
            MVADDCH(i+1, (j * 2 + 1), ch_L);
            MVADDCH(i+1, (j * 2 + 2), ch_R);
            if (color_on) attroff(COLOR_PAIR(GameInfo->field[i][j]));
        }
}

void printPause(GameInfo_t *GameInfo) {
    for (int i = FIELD_H / 2 - 4; i < FIELD_H / 2 + 1; i++)
        for (int j = 0; j < FIELD_W; j++) {
            MVADDCH(i+1, (j * 2 + 1), ' ');
            MVADDCH(i+1, (j * 2 + 2), ' ');
        }

    if (GameInfo->pause == 1) {// Пауза
        MVPRINTW(BOARD_H / 2 - 4, (BOARD_W - PAUSE_MESSAGE_LEN) / 2 + 1, PAUSE_MESSAGE);
        MVPRINTW(BOARD_H / 2 - 2, (BOARD_W - SMILE_WIDTH) / 2 + 1, SMILE_PAUSE);
        MVPRINTW(BOARD_H / 2, (BOARD_W - PRESS_P_MESSAGE_LEN) / 2 + 1, PRESS_P_MESSAGE);
    } else if (GameInfo->pause == 2) {// Победа
        MVPRINTW(BOARD_H / 2 - 4, (BOARD_W - WIN_MESSAGE_LEN) / 2 + 1, WIN_MESSAGE);
        MVPRINTW(BOARD_H / 2 - 2, (BOARD_W - SMILE_WIDTH) / 2 + 1, SMILE_WIN);
    } else if (GameInfo->pause == 3) {// Поражение
        MVPRINTW(BOARD_H / 2 - 4, (BOARD_W - LOSE_MESSAGE_LEN) / 2 + 1, LOSE_MESSAGE);
        MVPRINTW(BOARD_H / 2 - 2, (BOARD_W - SMILE_WIDTH) / 2 + 1, SMILE_SAD);
    }


    if (GameInfo->pause == 2 || GameInfo->pause == 3 || GameInfo->pause == 4) {
        MVPRINTW(BOARD_H / 2, (BOARD_W - PRESS_ENTER_MESSAGE_LEN) / 2 + 1, PRESS_ENTER_MESSAGE);
    }
    if (GameInfo->pause > 0) {
        MVPRINTW(BOARD_H / 2 + 1, (BOARD_W - PRESS_Q_MESSAGE_LEN) / 2 + 1 , PRESS_Q_MESSAGE);
    }
}


bool processKey(UserAction_t *action, bool *hold) {
    static int prev_key = 0;
    int key = getch();
    bool res = true;

    switch (key)
    {
    case KEY_START_R:
    case KEY_START_N:
        *action = Start;
        break;
    case KEY_PAUSE:
    case KEY_PAUSE_UPPERCASE:
        *action = Pause;
        break;
    case KEY_TERMINATE:
        *action = Terminate;
        break;
    case KEY_LEFT:
        *action = Left;
        break;
    case KEY_RIGHT:
        *action = Right;
        break;
    case KEY_UP:
        *action = Up;
        break;
    case KEY_DOWN:
        *action = Down;
        break;
    case KEY_ACTION:
        *action = Action;
        break;
    default:
        res = false;
        break;
    }

    if (res) {
        *hold = (key == prev_key);
        prev_key = key;
    } else {
        *hold = false;
        prev_key = 0;
    }

    return res;
}
