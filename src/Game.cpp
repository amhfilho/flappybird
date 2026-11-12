//
// Created by Filho, Antonio on 29/10/2026.
//

#include "Game.h"

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "AssetLoader.h"
#include "Constants.h"
#include "Colors.h"
#include "FontLoader.h"
#include "Random.h"
#include "TextureLoader.h"

Game::Game():
    state(GameState::Start),
    window(nullptr),
    renderer(nullptr),
    tileSet(Constants::Sprites::TileSetPath),
    bird(*this, {Constants::Sprites::Bird::StartX, Constants::Sprites::Bird::StartY}),
    lastPipeDeployTime(0),
    pipeDeployInterval(2000),
    score(0),
    best(0) {

    ticksCount = SDL_GetTicks();
}

Game::~Game() {
    font.reset(); // must be released before the renderer is destroyed
    for (MIX_Audio* audio : {flapSound, hitSound, gameOverSound, scoreSound}) {
        if (audio) MIX_DestroyAudio(audio);
    }
    if (mixer) MIX_DestroyMixer(mixer);
    MIX_Quit();
    TextureLoader::Clear();
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
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
        return false;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        SDL_Log("Error creating renderer: %s", SDL_GetError());
        return false;
    }

    if (!SDL_SetRenderVSync(renderer, 1)) {
        SDL_Log("Unable to enable vertical sync: %s", SDL_GetError());
        return false;
    }

    font = std::make_unique<FontLoader>(Constants::Font::Path, Constants::Font::Size);

    if (MIX_Init()) {
        mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    }

    if (mixer) {
        flapSound = MIX_LoadAudio(mixer, AssetLoader::GetAssetPath(Constants::Sound::Flap).c_str(), true);
        hitSound = MIX_LoadAudio(mixer, AssetLoader::GetAssetPath(Constants::Sound::Hit).c_str(), true);
        gameOverSound = MIX_LoadAudio(mixer, AssetLoader::GetAssetPath(Constants::Sound::GameOver).c_str(), true);
        scoreSound = MIX_LoadAudio(mixer, AssetLoader::GetAssetPath(Constants::Sound::Score).c_str(), true);
        if (!flapSound || !hitSound || !gameOverSound || !scoreSound) {
            SDL_Log("Some sounds failed to load: %s", SDL_GetError());
        }
    } else {
        SDL_Log("Audio unavailable: %s", SDL_GetError());
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
                    case SDLK_SPACE:
                        if (!event.key.repeat && state == GameState::Playing) {
                            bird.Flap();
                        }
                        break;
                }
                break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (state == GameState::Playing) {
                        bird.Flap();
                        PlaySound(flapSound);
                    }
                    else if (state == GameState::Start) {
                        state = GameState::Playing;
                        ticksCount = SDL_GetTicks();
                    }
                    else if (state == GameState::GameOver) {
                        state = GameState::Start;
                        Reset();
                    }
                }
                break;
        }
    }
}

void Game::Update() {
    if (state == GameState::Start || state == GameState::GameOver || state == GameState::Quit) {
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
        pipeDeployInterval = Random::Double(Constants::PipeDeployInterval::Min, Constants::PipeDeployInterval::Max);
    }

    for (auto& pair : pipesPairs) {
        pair.Update(deltaTime);
    }

    // Erase inactive pairs; the vector owns them by value, so their memory is released
    std::erase_if(pipesPairs, [](const PipesPair& pair) { return !pair.IsActive(); });

    CheckCollisions();

    UpdateScore();
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
            PlaySound(hitSound);
        }
    }
    if (bird.IsInFloor()) {
        PlaySound(gameOverSound);
        state = GameState::GameOver;
    }
}


void Game::Draw() {
    SDL_SetRenderDrawColor(renderer, lightBlue.r, lightBlue.g, lightBlue.b, lightBlue.a);
    SDL_RenderClear(renderer);

    background.Draw(renderer, tileSet);

    DrawStartScreen();

    for (auto& pair : pipesPairs) {
        pair.Draw(renderer, tileSet);
    }

    WriteScore();

    floor.Draw(renderer, tileSet);

    if (state != GameState::Start) {
        bird.Draw(*renderer, tileSet);
    }

    DrawGameOver();

    SDL_RenderPresent(renderer);
}

void Game::DrawStartScreen() {
    if (state == GameState::Start) {
        namespace StartScreen = Constants::Sprites::StartScreen;
        SDL_FRect startRect = {StartScreen::X, StartScreen::Y, StartScreen::Width, StartScreen::Height};
        float x = (Constants::Window::Width - startRect.w) / 2;
        SDL_FRect dstRect = {Constants::Start::X, Constants::Start::Y, startRect.w, startRect.h};

        SDL_Texture* texture = tileSet.GetTexture(renderer);
        SDL_RenderTexture(renderer, texture, &startRect, &dstRect);
    }
}

void Game::DrawGameOver() {
    if (state == GameState::GameOver) {
        namespace GameOver = Constants::Sprites::GameOver;
        SDL_FRect gameOverRect = {GameOver::X, GameOver::Y, GameOver::Width, GameOver::Height};
        SDL_FRect dstRect = {GameOver::CentralX, gameOverRect.y, gameOverRect.w, gameOverRect.h };

        SDL_Texture* texture = tileSet.GetTexture(renderer);
        SDL_RenderTexture(renderer, texture, &gameOverRect, &dstRect);

        if (score >= best) {
            best = score;
        }

        WriteScoreOnGameOver(score, GameOver::Score::Y);
        WriteScoreOnGameOver(best, GameOver::Best::Y);

        if (score >= 10 && score < 20) {
            DrawBronzeMedal(texture);
        }
        else if (score >= 20 && score < 30) {
            DrawSilverMedal(texture);
        }
        else if (score >= 30 && score < 40) {
            DrawGoldMedal(texture);
        }
        else if (score >= 40) {
            DrawPlatinumMedal(texture);
        }
    }
}

void Game::WriteScoreOnGameOver(int score, float y) {
    namespace GameOver = Constants::Sprites::GameOver;
    SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
    SDL_SetRenderScale(renderer, 2.0f, 2.0f);
    SDL_RenderDebugText(renderer, GameOver::Score::X, y, std::to_string(score).c_str());
    SDL_SetRenderScale(renderer, 1.0f, 1.0f);
}

void Game::DrawBronzeMedal(SDL_Texture* texture) {
    SDL_FRect bronzeSrc = {
        Constants::Sprites::Medal::Bronze::X,
        Constants::Sprites::Medal::Bronze::Y,
        Constants::Sprites::Medal::Bronze::Width,
        Constants::Sprites::Medal::Bronze::Height
    };
    DrawMedal(texture, bronzeSrc);
}

void Game::DrawSilverMedal(SDL_Texture* texture) {
    SDL_FRect silverSrc = {
        Constants::Sprites::Medal::Silver::X,
        Constants::Sprites::Medal::Silver::Y,
        Constants::Sprites::Medal::Silver::Width,
        Constants::Sprites::Medal::Silver::Height
    };
    DrawMedal(texture, silverSrc);
}

void Game::DrawGoldMedal(SDL_Texture* texture) {
    SDL_FRect goldSrc = {
        Constants::Sprites::Medal::Gold::X,
        Constants::Sprites::Medal::Gold::Y,
        Constants::Sprites::Medal::Gold::Width,
        Constants::Sprites::Medal::Gold::Height
    };
    DrawMedal(texture, goldSrc);
}

void Game::DrawPlatinumMedal(SDL_Texture* texture) {
    SDL_FRect goldSrc = {
        Constants::Sprites::Medal::Platinum::X,
        Constants::Sprites::Medal::Platinum::Y,
        Constants::Sprites::Medal::Platinum::Width,
        Constants::Sprites::Medal::Platinum::Height
    };
    DrawMedal(texture, goldSrc);
}

void Game::DrawMedal(SDL_Texture* texture, SDL_FRect medalSrc) {
    namespace GameOver = Constants::Sprites::GameOver;
    SDL_FRect medalDst = {GameOver::Medal::X, GameOver::Medal::Y, medalSrc.w, medalSrc.h};
    SDL_RenderTexture(renderer, texture, &medalSrc, &medalDst);
}

void Game::WriteScore() {
    if (state != GameState::Start && state != GameState::GameOver) {
        std::string text = std::to_string(score);
        const float x = (Constants::Window::Width - font->MeasureTextWidth(text)) / 2.0f;
        // DrawText anchors the bottom edge, so add the font height to keep the top at topMargin
        const float bottomY = Constants::Score::TopMargin + static_cast<float>(font->GetHeight());
        font->DrawText(renderer, text, x, bottomY, white);
    }
}

void Game::UpdateScore() {
    if (state == GameState::Playing) {
        const SDL_FRect birdRect = bird.GetRect();
        for (auto& pair : pipesPairs) {
            if (pair.IsActive()) {
                const SDL_FRect scoreRect = pair.GetScoreRect();
                // Score only once the bird has fully left the gap on the right side
                // (a collision would have ended the Playing state before this point).
                if (!pair.IsScored() && birdRect.x > scoreRect.x + scoreRect.w) {
                    score++;
                    pair.MarkScored();
                    PlaySound(scoreSound);
                }
            }
        }
    }
}

std::uint64_t Game::GetTicksCount() const {
    return ticksCount;
}

Game::GameState Game::GetGameState() const {
    return state;
}

void Game::PlaySound(MIX_Audio* sound) {
    if (mixer && sound) {
        MIX_PlayAudio(mixer, sound);
    }
}

void Game::Reset() {
    state = GameState::Start;
    bird.Reset();
    pipesPairs.clear();
    lastPipeDeployTime = 0;
    pipeDeployInterval = 0;
    freezeUntil = 0;
    score = 0;
}
