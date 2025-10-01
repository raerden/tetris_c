#ifndef BRICK_GAME_H
#define BRICK_GAME_H

#include <stdbool.h>

#define FIELD_W     10
#define FIELD_H     20
#define FIGURE_FIELD_SIZE 4

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState();

#endif