//
// Created by Filho, Antonio on 29/10/2026.
//

#include "Game.h"

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "Constants.h"
#include "Colors.h"
#include "TextureLoader.h"

Game::Game():
    state(GameState::Start),
    window(nullptr),
    renderer(nullptr),
    bird(*this, {50.0f, 50.0f}),
    tileSet(Constants::Sprites::TileSetPath),
    pipesPair({0.0f, 0.0f}) {

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

void Game::LoadData() {

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
                        if (!event.key.repeat) {
                            bird.Flap();
                        }
                        break;
                }
                break;

        }
    }
}

void Game::Update() {
    if (state == GameState::GameOver) {
        return;
    }
    Uint64 currentTicks = SDL_GetTicks();
    deltaTime = static_cast<float>(currentTicks - ticksCount) / 1000.0f;
    ticksCount = currentTicks;

    floor.Update(deltaTime);
    bird.Update(deltaTime);
    pipesPair.Update(deltaTime);

    CheckCollisions();
}

void Game::CheckCollisions() {
    const UpperPipe& upperPipe = pipesPair.GetUpperPipe();
    const DownPipe& downPipe = pipesPair.GetDownPipe();
    SDL_FRect upperRect = upperPipe.GetRect();
    SDL_FRect downRect = downPipe.GetRect();

    const SDL_FRect birdRect = bird.GetRect();

    if (SDL_HasRectIntersectionFloat(&birdRect, &upperRect) ||
        SDL_HasRectIntersectionFloat(&birdRect, &downRect)) {
        //state = GameState::GameOver;
        SDL_Log("Bird collided with a pipe!");
    }
}

void Game::Draw() {
    SDL_SetRenderDrawColor(renderer, lightBlue.r, lightBlue.g, lightBlue.b, lightBlue.a);
    SDL_RenderClear(renderer);

    background.Draw(renderer);
    pipesPair.Draw(renderer);
    floor.Draw(renderer);
    bird.Draw(*renderer);

    SDL_RenderPresent(renderer);
}

std::uint64_t Game::GetTicksCount() const {
    return ticksCount;
}
