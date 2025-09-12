#ifndef BRICKGAME_FRONTEND_H
#define BRICKGAME_FRONTEND_H

#include <ncurses.h>
// #include <stdlib.h>
// #include <string.h>
#include "defines.h"
#include "../../brick_game.h"
// #include "objects.h"

void win_init(int time);
void win_close();
void print_board(void);
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);
void clear_field();
void print_field(GameInfo_t *game);
void print_stats(GameInfo_t *game);
void print_next(GameInfo_t *game);
void print_pause(GameInfo_t *game);

#endif