//
// Created by Filho, Antonio on 29/10/2026.
//

#pragma once
#include "Vector2.h"

class Game;
struct SDL_FRect;
struct SDL_Renderer;

class Bird {
public:
    Bird(Game& game, Vector2 position);
    ~Bird() = default;

    void Update(float deltaTime);
    void Draw(SDL_Renderer& renderer);

    void Flap();

    SDL_FRect GetRect();

private:
    Game& game;
    Vector2 position;
    float velocity;
    float gravity;
};
