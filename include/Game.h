//
// Created by Filho, Antonio on 29/10/2026.
//

#pragma once
#include <cstdint>

#include "Background.h"
#include "Bird.h"
#include "DownPipe.h"
#include "Floor.h"
#include "Sprite.h"
#include "UpperPipe.h"

struct SDL_Renderer;
struct SDL_Window;
struct MIX_Mixer;

class Game {

public:
    enum class GameState {
        Start, Playing, GameOver, Quit
    };

    Game();
    ~Game();

    GameState state;

    bool InitializeWindow();
    void LoadData();
    void RunLoop();
    [[nodiscard]] std::uint64_t GetTicksCount() const;

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
    DownPipe downPipe;
    UpperPipe upperPipe;

    void HandleInput();
    void Update();
    void Draw();
};

