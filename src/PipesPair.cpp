//
// Created by Antonio Mario Henriques Filho on 01/11/2026.
//

#include "PipesPair.h"

#include "Constants.h"
#include "Random.h"

#include <SDL3/SDL_log.h>
#include <algorithm>
#include <SDL3/SDL_rect.h>

PipesPair::PipesPair(Vector2 position) :
    position(position),
    upperPipe({0.0f, 0.0f}),
    downPipe({0.0f, 0.0f}),
    active(true) {

    float x = Constants::Window::Width;

    namespace Sprites = Constants::Sprites;

    // Fixed gap: downY is derived from upperY, so no search is needed.
    // Restrict upperY so both pipes stay inside their valid Y ranges.
    const float gapOffset = Sprites::UpperPipe::Height + Sprites::Pipes::MinimumInitialGap;
    const float minUpperY = std::max(Sprites::UpperPipe::MinimumY, Sprites::DownPipe::MinimumY - gapOffset);
    const float maxUpperY = std::min(Sprites::UpperPipe::MaximumY, Sprites::DownPipe::MaximumY - gapOffset);

    const float upperY = Random::Float(minUpperY, maxUpperY);
    const float downY = upperY + gapOffset;
    upperPipe.SetPosition({x, upperY});
    downPipe.SetPosition({x, downY});
};

void PipesPair::Update(float deltaTtime) {
    if (!active) return;
    Vector2 upperPosition = upperPipe.GetPosition();
    Vector2 downPosition = downPipe.GetPosition();
    upperPosition.x -= deltaTtime * Constants::Sprites::Pipes::Speed;
    downPosition.x -= deltaTtime * Constants::Sprites::Pipes::Speed;
    upperPipe.SetPosition(upperPosition);
    downPipe.SetPosition(downPosition);
    // Inactive once fully off the left edge of the screen
    active = upperPipe.GetPosition().x + Constants::Sprites::UpperPipe::Width > 0;
}

void PipesPair::Draw(SDL_Renderer *renderer, Sprite& tileSet) {
    if (!active) return;
    upperPipe.Draw(renderer, tileSet);
    downPipe.Draw(renderer, tileSet);
}

SDL_FRect PipesPair::GetScoreRect() const {
    const Vector2 upperPosition = upperPipe.GetPosition();
    const Vector2 downPosition = downPipe.GetPosition();
    const float gapTop = upperPosition.y + Constants::Sprites::UpperPipe::Height;

    return SDL_FRect{
        upperPosition.x,
        gapTop,
        Constants::Sprites::UpperPipe::Width,
        downPosition.y - gapTop
    };
}
