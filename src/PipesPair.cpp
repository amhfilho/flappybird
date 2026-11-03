//
// Created by Antonio Mario Henriques Filho on 01/11/2026.
//

#include "PipesPair.h"

#include "Constants.h"

#include <random>

PipesPair::PipesPair(Vector2 position) :
    position(position),
    upperPipe({0.0f, 0.0f}),
    downPipe({0.0f, 0.0f}) {

    float x = Constants::Window::Width;
    float minimumY = Constants::Sprites::Floor::Y - 62.5f;
    float maximumY = 62.5f;
    float minimumUpperYOffset = 12.5f;
    float minimumLowerYOffset = 12.5f;

    float centerY = RandomFloat(minimumY, maximumY);
    upperPipe.setPosition

};

float PipesPair::RandomFloat(float min, float max) {
    static std::mt19937 generator{std::random_device{}()};
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(generator);
}

void PipesPair::Draw(SDL_Renderer *renderer) {

}
