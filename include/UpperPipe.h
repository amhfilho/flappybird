//
// Created by Antonio Mario Henriques Filho on 01/11/2026.
//
#pragma once
#include "Vector2.h"

struct SDL_FRect;
struct SDL_Renderer;
class Sprite;

class UpperPipe {
public:
    UpperPipe(Vector2 position);
    ~UpperPipe() = default;

    void Update(float deltaTime);
    void Draw(SDL_Renderer* renderer, Sprite& tileSet);

    Vector2 GetPosition() const;

    void SetPosition(Vector2 position);

    SDL_FRect GetRect() const;

private:
    Vector2 position;
};
