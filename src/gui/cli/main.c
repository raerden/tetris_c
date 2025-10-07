#include "../../brick_game.h"
#include "../../brick_game/tetris/backend.h"
#include "./frontend.h"

int main(void) {
  winInit();

  GameInfo_t gameInfo;

  while (userAction() != Terminate) {
    gameInfo = updateCurrentState();
    renderGame(&gameInfo);
  }

  winClose();

  return 0;
}
