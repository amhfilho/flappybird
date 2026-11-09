//
// Created by Filho, Antonio on 29/10/2026.
//

#pragma once
#include <cstdint>

#include "Background.h"
#include "Bird.h"
#include "DownPipe.h"
#include "Floor.h"
#include "PipesPair.h"
#include "Sprite.h"
#include "UpperPipe.h"

struct SDL_Renderer;
struct SDL_Window;
struct MIX_Mixer;

class Game {

public:
    enum class GameState {
        Start, Playing, BirdHurt, GameOver, Quit
    };

    Game();
    ~Game();

    GameState state;

    bool InitializeWindow();
    void RunLoop();
    [[nodiscard]] std::uint64_t GetTicksCount() const;
    GameState GetGameState() const;

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    MIX_Mixer* mixer = nullptr;

    std::uint64_t ticksCount;
    float deltaTime;

    Bird bird;
    Sprite tileSet;
    Background background;
    Floor floor;

    std::vector<PipesPair> pipesPairs;
    Uint64 lastPipeDeployTime;
    int pipeDeployInterval;
    Uint64 freezeUntil = 0;

    void HandleInput();
    void Update();

    void CheckCollisions();

    void DrawGameOver();

    void Draw();
};

