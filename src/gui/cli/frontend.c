#include "frontend.h"

void win_init(int time) {
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(time);
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_BLACK, COLR_RED);    // палка
        init_pair(2, COLOR_BLACK, COLR_ORANGE); // Г-правая - оранжевая
        init_pair(3, COLOR_BLACK, COLR_YELLOW); // Г-левая - желтая
        init_pair(4, COLOR_BLACK, COLR_PINK);   // квадрат - розовый
        init_pair(5, COLOR_BLACK, COLR_GREEN);  // z-правая зеленая
        init_pair(6, COLOR_BLACK, COLR_BLUE);   // T - синяя
        init_pair(7, COLOR_BLACK, COLR_PURPLE); // z-левая пурпурная
    }
}

void win_close() {
    reset_shell_mode();
    echo();
    nocbreak();
    keypad(stdscr, FALSE);
    endwin();
}

void print_overlay(void) {
    // игровое поле
    print_rectangle(0, BOARD_H + 1, 0, BOARD_W + 1);
    // поле статистики
    print_rectangle(0, BOARD_H + 1, BOARD_W + 2, BOARD_W + INFO_W + 3);
   
    print_rectangle(1, 3, BOARD_W + 3, BOARD_W + INFO_W + 2);
    MVPRINTW(2, BOARD_W + 4, "BEST");

    print_rectangle(4, 6, BOARD_W + 3, BOARD_W + INFO_W + 2);
    MVPRINTW(5, BOARD_W + 4, "SCORE");

    print_rectangle(7, 9, BOARD_W + 3, BOARD_W + INFO_W + 2);
    MVPRINTW(8, BOARD_W + 4, "LEVEL");
    
    // print_rectangle(11, 16, BOARD_W + 4, BOARD_W + INFO_W + 1);
    MVPRINTW(10, BOARD_W + 7, "NEXT");

    MVPRINTW(BOARD_H / 2, (BOARD_W - INTRO_MESSAGE_LEN) / 2 + 1, INTRO_MESSAGE);
}

void print_rectangle(int top_y, int bottom_y, int left_x, int right_x)
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

void print_next(GameInfo_t *game) {
    // очистка от предыдущей фигуры
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            MVADDCH(12 + i, BOARD_W + 4 + (j * 2 + 1), ' ');
            MVADDCH(12 + i, BOARD_W + 4 + (j * 2 + 2), ' ');
        }

    // найти цвет фигурки в матрице **next
    int color = 0;
    for (int i = 0; i < 4; i++) 
        if (game->next[0][i] > 0) {
            color = game->next[0][i];
            break;
        }

    attron(COLOR_PAIR(color));
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) 
            if (game->next[i][j]) {
                MVADDCH(12 + i, BOARD_W + 4 + (j * 2 + 1), ' ');
                MVADDCH(12 + i, BOARD_W + 4 + (j * 2 + 2), ' ');
        }
    attroff(COLOR_PAIR(color));
}

void print_stats(GameInfo_t *game) {
    MVPRINTW(2, BOARD_W + 10, "    ");
    MVPRINTW(2, BOARD_W + 10, "%-d", game->high_score);

    MVPRINTW(5, BOARD_W + 10, "    ");
    MVPRINTW(5, BOARD_W + 10, "%-d", game->score);

    MVPRINTW(8, BOARD_W + 10, "  ");
    MVPRINTW(8, BOARD_W + 10, "%-d", game->level);

    print_next(game);
}

void clear_field() {
    for (int i = 0; i < FIELD_H; i++)
        for (int j = 0; j < FIELD_W; j++) {
            MVADDCH(i+1, (j * 2 + 1), ' ');
            MVADDCH(i+1, (j * 2 + 2), ' ');
        }
}

void print_field(GameInfo_t *game) {
    for (int i = 0; i < BOARD_H; i++)
        for (int j = 0; j < FIELD_W; j++) {
            attron(COLOR_PAIR(game->field[i][j]));
            MVADDCH(i+1, (j * 2 + 1), ' ');
            MVADDCH(i+1, (j * 2 + 2), ' ');
            attroff(COLOR_PAIR(game->field[i][j]));
        }
}

void print_pause(GameInfo_t *game) {
    clear_field();
    if (game->pause == 1) {
        MVPRINTW(BOARD_H / 2 - 4, (BOARD_W - 13) / 2 + 1, "Game on pause");
        MVPRINTW(BOARD_H / 2 - 2, (BOARD_W - 5) / 2 + 1, "(O_~)");
        MVPRINTW(BOARD_H / 2, (BOARD_W - 19) / 2 + 1, "Press P to continue");
    } else if (game->pause == 2) {
        MVPRINTW(BOARD_H / 2 - 4, (BOARD_W - 13) / 2 + 1, "!! YOU WON !!");
        MVPRINTW(BOARD_H / 2 - 2, (BOARD_W - 5) / 2 + 1, "(^_^)");
    } else if (game->pause == 3) {
        MVPRINTW(BOARD_H / 2 - 4, (BOARD_W - 9) / 2 + 1, "GAME OVER");
        MVPRINTW(BOARD_H / 2 - 2, (BOARD_W - 5) / 2 + 1, "(~_~)");
    }
    if (game->pause == 2 || game->pause == 3) {
        MVPRINTW(BOARD_H / 2, (BOARD_W - 20) / 2 + 1, "Press Enter to start");
        MVPRINTW(BOARD_H / 2 + 1, (BOARD_W - 15) / 2 + 1, "Press Q to quit");
    }
}
