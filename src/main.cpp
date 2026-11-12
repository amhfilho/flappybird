#include "Game.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

void WebMainLoop(void* arg) {
    Game* game = static_cast<Game*>(arg);

    if (!game->IsRunning()) {
#ifdef __EMSCRIPTEN__
        emscripten_cancel_main_loop();
#endif
        return;
    }

    game->RunFrame();
}

int main() {
    Game game;
    if (!game.InitializeWindow()) {
        return -1;
    }

#ifdef __EMSCRIPTEN__
    // 0 = usa a taxa de atualização do navegador (requestAnimationFrame)
    // 1 = simula loop infinito para não destruir o objeto 'game' da stack
    emscripten_set_main_loop_arg(WebMainLoop, &game, 0, 1);
#else
    // Seu código original intacto para macOS / Xcode
    game.RunLoop();
#endif

    return 0;
}