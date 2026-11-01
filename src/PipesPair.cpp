//
// Created by Antonio Mario Henriques Filho on 01/11/2026.
//

#include "PipesPair.h"

#include "Constants.h"

PipesPair::PipesPair(Vector2 position) :
    position(position),
    upperPipe({0.0f, 0.0f}),
    downPipe({0.0f, 0.0f}) {

    float x = Constants::Window::Width;
    float minimumY = Constants::Sprites::Floor::Y - 62.5f;
    float maximumY = 62.5f;
    float minimumUpperYOffset = 12.5f;
    float minimumLowerYOffset = 12.5f;


};

void PipesPair::Draw(SDL_Renderer *renderer) {

}
