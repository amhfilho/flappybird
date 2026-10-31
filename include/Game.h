//
// Created by Filho, Antonio on 29/10/2026.
//

#pragma once
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "Bird.h"
#include "Bird.h"

class Game {

public:
    enum class GameState {
        Start, Playing, GameOver, Quit
    };

    Game();
    ~Game() = default;

    GameState state;

    bool InitializeWindow();
    void RunLoop();
    [[nodiscard]] Uint64 GetTicksCount() const;

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    MIX_Mixer* mixer = nullptr;

    Uint64 ticksCount;
    float deltaTime;

    Bird bird;

    void HandleInput();
    void Update();
    void Draw();
};

