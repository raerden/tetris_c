#ifndef BRICKGAME_FRONTEND_H
#define BRICKGAME_FRONTEND_H

#include <ncurses.h>
#include "defines.h"
#include "../../brick_game.h"


void win_init(int time);
void win_close();
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);
void print_board(void);
void clear_field();
void print_field(GameInfo_t *GameInfo);
void print_stats(GameInfo_t *GameInfo);
void print_next(GameInfo_t *GameInfo);
void print_pause(GameInfo_t *GameInfo);
bool process_key(UserAction_t *action, bool *hold);

#endif