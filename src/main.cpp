#include "Game.h"

int main() {
    Game game;
    if (!game.InitializeWindow()) {
        return -1;
    }

    game.RunLoop();

    return 0;
}
