#include "../../brick_game.h"
#include "./frontend.h"
#include "../../brick_game/tetris/backend.h"

int main() {
    winInit();
    
    GameInfo_t gameInfo = {0};

    while(userAction() != Terminate) {
        gameInfo = updateCurrentState();
        renderGame(&gameInfo);
    }

    winClose();

    return 0;
}