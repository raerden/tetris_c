#include "frontend.h"

void print_overlay(void)
{
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

void print_levelerror(void)
{
    clear();
    MVPRINTW(0, 0, "An error occured openning level file!");
    MVPRINTW(2, 0, "Please check ./tests/ directory.");
    MVPRINTW(3, 0, "There should be 5 level files named level_(1-5).txt.");
    MVPRINTW(4, 0, "Also try to open the game nearby ./tests/ directory.");
    MVPRINTW(6, 0, "Press any key to exit.");
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
    MVPRINTW(2, BOARD_W + 10, "%-d", game->high_score);
    MVPRINTW(5, BOARD_W + 10, "%-d", game->score);
    MVPRINTW(8, BOARD_W + 10, "%-d", game->level);

    print_next(game);
    
//init_pair(1, COLOR_BLACK, COLOR_RED);
/* Цвета ncurses
#define COLOR_RED	1      палка
#define COLOR_GREEN	2      N
#define COLOR_YELLOW	3  Г
#define COLOR_BLUE	4      Т
#define COLOR_MAGENTA	5  box
#define COLOR_CYAN	6
#define COLOR_WHITE	7

*/
}

void print_board(GameInfo_t *game) {
    for (int i = 0; i < BOARD_H; i++)
        for (int j = 0; j < FIELD_W; j++) 
            if (game->field[i][j]) {
                attron(COLOR_PAIR(game->field[i][j]));
                MVADDCH(i+1, (j * 2 + 1), ' ');
                MVADDCH(i+1, (j * 2 + 2), ' ');
                attroff(COLOR_PAIR(game->field[i][j]));
            }
}

/*
void print_stats(game_stats_t *stats)
{
    MVPRINTW(2, BOARD_W + 12, "%d", stats->level);
    MVPRINTW(5, BOARD_W + 12, "%d", stats->score);
    MVPRINTW(8, BOARD_W + 12, "%d", stats->speed);
    MVPRINTW(11, BOARD_W + 12, "%d", stats->lives);
}

void print_board(board_t *game, player_pos *frog)
{
    print_cars(game);
    PRINT_FROG(frog->x, frog->y);
}


void print_finished(board_t *game)
{
    for (int i = 0; i < BOARD_W; i++)
    {
        if (game->finish[i] == '0')
            MVADDCH(1, i + 1, ACS_BLOCK);
        else
            MVADDCH(1, i + 1, ' ');
    }       
}
void print_banner(game_stats_t *stats)
{
    banner_t banner;

    memset(banner.matrix, 0, (BANNER_N + 1) * (BANNER_M + 1));

    clear();
    
    if (!(read_banner(stats, &banner)))
    {
        for (int i = 0; i < BANNER_N; i++)
            for (int j = 0; j < BANNER_M; j++)
                if (banner.matrix[i][j] == '#')
                    MVADDCH(i, j, ACS_BLOCK);
                else
                    MVADDCH(i, j, ' ');
        refresh();
        napms(2000);
    }
}

int read_banner(game_stats_t *stats, banner_t *banner)
{
    int rc = SUCCESS;
    FILE *file = NULL;

    if (stats->lives)
        file = fopen(YOU_WON, "r");
    else
        file = fopen(YOU_LOSE, "r");

    if (file)
    {
        for (int i = 0; i < BANNER_N - 1 && !rc; i++)
        {
            if (fgets(banner->matrix[i], BANNER_M + 2, file) == NULL)
                rc = ERROR;
            else
                banner->matrix[i][strcspn(banner->matrix[i], "\n")] = '\0';
        }

        fclose(file);
    }
    else
        rc = ERROR;

    return rc;
}
*/