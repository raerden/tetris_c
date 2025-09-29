#include "./brick_game.h"
#include "./gui/cli/frontend.h"
#include "./brick_game/tetris/backend.h"

int main() {
    winInit();
    
    GameInfo_t gameInfo = {0};

    while(isGameRunning()) {
        userAction();                   // Frontend
        stepGame();                     // Backend
        gameInfo = updateCurrentState();// Backend
        renderGame(&gameInfo);          // Frontend
    }

    terminateGame(&gameInfo);
    winClose();

    return 0;
}