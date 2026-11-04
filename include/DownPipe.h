//
// Created by Antonio Mario Henriques Filho on 01/11/2026.
//
#pragma once

#include "Vector2.h"

struct SDL_FRect;
struct SDL_Renderer;

class DownPipe {
public:
    DownPipe(Vector2 position);
    ~DownPipe() = default;

    void Update(float deltaTime);
    void Draw(SDL_Renderer* renderer);

    void SetPosition(Vector2 position);

    SDL_FRect GetRect() const;

    Vector2 GetPosition() const;

private:
    Vector2 position;
};
