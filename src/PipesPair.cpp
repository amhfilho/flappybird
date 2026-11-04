//
// Created by Antonio Mario Henriques Filho on 01/11/2026.
//

#include "PipesPair.h"

#include "Constants.h"

#include <random>
#include <SDL3/SDL_log.h>

PipesPair::PipesPair(Vector2 position) :
    position(position),
    upperPipe({0.0f, 0.0f}),
    downPipe({0.0f, 0.0f}) {

    float x = Constants::Window::Width;

    float gap = 0.0f;
    float upperY = 0;
    float downY = 0;
    while (gap < 50.0f) {
        upperY = RandomFloat(Constants::Sprites::UpperPipe::MinimumY, Constants::Sprites::UpperPipe::MaximumY);
        downY = RandomFloat(Constants::Sprites::DownPipe::MinimumY, Constants::Sprites::DownPipe::MaximumY);
        gap = downY - (upperY + Constants::Sprites::UpperPipe::Height);
    }
    SDL_Log("upperY: %f", upperY);
    SDL_Log("downY: %f", downY);
    SDL_Log("gap: %f", gap);
    upperPipe.SetPosition({x, upperY});
    downPipe.SetPosition({x, downY});
};

float PipesPair::RandomFloat(float min, float max) {
    static std::mt19937 generator{std::random_device{}()};
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(generator);
}

void PipesPair::Update(float deltaTtime) {
    Vector2 upperPosition = upperPipe.GetPosition();
    Vector2 downPosition = downPipe.GetPosition();
    upperPosition.x -= deltaTtime * Constants::Sprites::Pipes::Speed;
    downPosition.x -= deltaTtime * Constants::Sprites::Pipes::Speed;
    upperPipe.SetPosition(upperPosition);
    downPipe.SetPosition(downPosition);
}

void PipesPair::Draw(SDL_Renderer *renderer) {
    upperPipe.Draw(renderer);
    downPipe.Draw(renderer);
}
