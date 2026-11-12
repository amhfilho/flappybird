//
// Created by Filho, Antonio on 29/10/2026.
//

#pragma once
#include <cstdint>
#include <memory>
#include <SDL3_mixer/SDL_mixer.h>

#include "Background.h"
#include "Bird.h"
#include "DownPipe.h"
#include "Floor.h"
#include "PipesPair.h"
#include "Sprite.h"
#include "UpperPipe.h"

class FontLoader;
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

    bool IsRunning();
    void RunFrame();

    GameState state;

    bool InitializeWindow();
    void RunLoop();
    [[nodiscard]] std::uint64_t GetTicksCount() const;
    GameState GetGameState() const;

    void PlaySound(MIX_Audio *sound);

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    MIX_Mixer* mixer = nullptr;
    MIX_Audio* flapSound = nullptr;
    MIX_Audio* hitSound = nullptr;
    MIX_Audio* gameOverSound = nullptr;
    MIX_Audio* scoreSound = nullptr;


    std::uint64_t ticksCount;
    float deltaTime;

    Sprite tileSet; // the only instance of the main sprite sheet, shared by every drawable
    Bird bird;
    Background background;
    Floor floor;

    std::vector<PipesPair> pipesPairs;
    Uint64 lastPipeDeployTime;
    int pipeDeployInterval;
    Uint64 freezeUntil = 0;

    int score;
    int best;

    std::unique_ptr<FontLoader> font;

    void Reset();

    void HandleInput();

    void UpdateScore();

    void Update();

    void CheckCollisions();

    void DrawMedal(SDL_Texture* texture, SDL_FRect bronze_src);

    void DrawBronzeMedal(SDL_Texture* texture);

    void DrawSilverMedal(SDL_Texture *texture);

    void DrawGoldMedal(SDL_Texture * texture);

    void DrawPlatinumMedal(SDL_Texture * texture);

    void DrawGameOver();

    void WriteScoreOnGameOver(int score, float y);

    void WriteScore();

    void Draw();

    void DrawStartScreen();
};

