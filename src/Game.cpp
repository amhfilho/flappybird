//
// Created by Filho, Antonio on 29/10/2026.
//

#include "Game.h"

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "Constants.h"
#include "Colors.h"
#include "Random.h"
#include "TextureLoader.h"

Game::Game():
    state(GameState::Start),
    window(nullptr),
    renderer(nullptr),
    bird(*this, {50.0f, 50.0f}),
    tileSet(Constants::Sprites::TileSetPath),
    lastPipeDeployTime(0),
    pipeDeployInterval(2000) {

    ticksCount = SDL_GetTicks();
}

Game::~Game() {
    TextureLoader::Clear();
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
}

bool Game::InitializeWindow() {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("Error starting SDL3: %s", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow(
        Constants::Window::Title,
        Constants::Window::Width,
        Constants::Window::Height,
        0);
    if (!window) {
        SDL_Log("Error creating window: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        SDL_Log("Error creating renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    if (!SDL_SetRenderVSync(renderer, 1)) {
        SDL_Log("Unable to enable vertical sync: %s", SDL_GetError());
        return false;
    }

    return true;
}

void Game::RunLoop()
{
    while (state != GameState::Quit)
    {
        HandleInput();
        Update();
        Draw();
    }
}

void Game::HandleInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                state = GameState::Quit;
                break;
            case SDL_EVENT_KEY_DOWN:
                switch (event.key.key) {
                    case SDLK_ESCAPE:
                        state = GameState::Quit;
                        break;
                    case SDLK_UP:
                        if (!event.key.repeat && state != GameState::GameOver) {
                            bird.Flap();
                        }
                        break;
                }
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                if (event.button.button == SDL_BUTTON_LEFT && state != GameState::GameOver) {
                    bird.Flap();
                }
                break;
        }
    }
}

void Game::Update() {
    if (state == GameState::GameOver || state == GameState::Quit) {
        return;
    }
    Uint64 currentTicks = SDL_GetTicks();
    deltaTime = static_cast<float>(currentTicks - ticksCount) / 1000.0f;
    ticksCount = currentTicks;

    // Freeze the world briefly after hitting a pipe. ticksCount keeps advancing
    // so deltaTime doesn't spike when the freeze ends.
    if (currentTicks < freezeUntil) {
        return;
    }

    floor.Update(deltaTime);
    bird.Update(deltaTime);

    if (currentTicks - lastPipeDeployTime > pipeDeployInterval) {
        pipesPairs.emplace_back(Vector2{0.0f, 0.0f});
        lastPipeDeployTime = currentTicks;
        pipeDeployInterval = Random::Double(2000, 3500);
    }

    for (auto& pair : pipesPairs) {
        pair.Update(deltaTime);
    }

    // Erase inactive pairs; the vector owns them by value, so their memory is released
    std::erase_if(pipesPairs, [](const PipesPair& pair) { return !pair.IsActive(); });

    CheckCollisions();
}

void Game::CheckCollisions() {
    //Collision with pipe
    for (auto& pair : pipesPairs) {
        const UpperPipe& upperPipe = pair.GetUpperPipe();
        const DownPipe& downPipe = pair.GetDownPipe();
        SDL_FRect upperRect = upperPipe.GetRect();
        SDL_FRect downRect = downPipe.GetRect();

        const SDL_FRect birdRect = bird.GetRect();

        if (SDL_HasRectIntersectionFloat(&birdRect, &upperRect) ||
            SDL_HasRectIntersectionFloat(&birdRect, &downRect)) {
            if (state != GameState::BirdHurt) {
                freezeUntil = SDL_GetTicks() + Constants::HitFreezeMs;
            }
            state = GameState::BirdHurt;
            bird.OnPipeCollide();
        }
    }
    if (bird.IsInFloor()) {
        state = GameState::GameOver;
    }
}


void Game::Draw() {
    SDL_SetRenderDrawColor(renderer, lightBlue.r, lightBlue.g, lightBlue.b, lightBlue.a);
    SDL_RenderClear(renderer);

    background.Draw(renderer);

    for (auto& pair : pipesPairs) {
        pair.Draw(renderer);
    }

    floor.Draw(renderer);
    bird.Draw(*renderer);

    DrawGameOver();

    SDL_RenderPresent(renderer);
}

void Game::DrawGameOver() {
    if (state == GameState::GameOver) {
        namespace GameOver = Constants::Sprites::GameOver;
        SDL_FRect gameOverRect = {GameOver::X, GameOver::Y, GameOver::Width, GameOver::Height};
        SDL_FRect dstRect = {Constants::Window::Width / 2-gameOverRect.w / 2,
            gameOverRect.y,
            gameOverRect.w,
            gameOverRect.h };

        SDL_Texture* texture = TextureLoader::LoadTexture(renderer, Constants::Sprites::TileSetPath);
        SDL_RenderTexture(renderer, texture, &gameOverRect, &dstRect);
    }
}

std::uint64_t Game::GetTicksCount() const {
    return ticksCount;
}

Game::GameState Game::GetGameState() const {
    return state;
}
