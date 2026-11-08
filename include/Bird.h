//
// Created by Filho, Antonio on 29/10/2026.
//

#pragma once
#include <vector>
#include <SDL3/SDL_render.h>

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

    SDL_FRect GetRect() const;

private:
    Game& game;
    Vector2 position;
    float velocity;
    float gravity;

    std::vector<SDL_FRect> spriteRects;
    float currentFrame;
};
