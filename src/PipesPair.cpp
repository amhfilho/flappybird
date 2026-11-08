//
// Created by Antonio Mario Henriques Filho on 01/11/2026.
//

#include "PipesPair.h"

#include "Constants.h"
#include "Random.h"

#include <SDL3/SDL_log.h>

PipesPair::PipesPair(Vector2 position) :
    position(position),
    upperPipe({0.0f, 0.0f}),
    downPipe({0.0f, 0.0f}),
    active(true) {

    float x = Constants::Window::Width;

    float gap = 0.0f;
    float upperY = 0;
    float downY = 0;
    while (gap < 50.0f) {
        upperY = Random::Float(Constants::Sprites::UpperPipe::MinimumY, Constants::Sprites::UpperPipe::MaximumY);
        downY = Random::Float(Constants::Sprites::DownPipe::MinimumY, Constants::Sprites::DownPipe::MaximumY);
        gap = downY - (upperY + Constants::Sprites::UpperPipe::Height);
    }
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

void PipesPair::Draw(SDL_Renderer *renderer) {
    if (!active) return;
    upperPipe.Draw(renderer);
    downPipe.Draw(renderer);
}
