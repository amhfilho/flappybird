//
// Created by Antonio Mario Henriques Filho on 01/11/2026.
//
#pragma once
#include "Vector2.h"

struct SDL_FRect;
struct SDL_Renderer;

class UpperPipe {
public:
    UpperPipe(Vector2 position);
    ~UpperPipe() = default;

    void Update(float deltaTime);
    void Draw(SDL_Renderer* renderer);

    Vector2 GetPosition() const;

    SDL_FRect GetRect();

private:
    Vector2 position;
};
