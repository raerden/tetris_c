#ifndef BRICKGAME_FRONTEND_H
#define BRICKGAME_FRONTEND_H

#include <ncurses.h>
#include "defines.h"
#include "../../brick_game.h"


void winInit();
void winClose();
void printRectangle(int top_y, int bottom_y, int left_x, int right_x);
void printBoard();
void clearField();
void printField(GameInfo_t *GameInfo);
void printStats(GameInfo_t *GameInfo);
void printNext(GameInfo_t *GameInfo);
void printPause(GameInfo_t *GameInfo);
void renderGame(GameInfo_t *GameInfo);

bool processKey(UserAction_t *action, bool *hold);
void userAction();

#endif