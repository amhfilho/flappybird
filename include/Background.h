//
// Created by (Admin) Antonio Filho on 31/10/2026.
//

#pragma once
#include "Sprite.h"

class Background {
public:
    Background();
    ~Background() = default;

    void Update(float deltaTime);
    void Draw(SDL_Renderer* renderer);

    float GetFloorY();

private:
    Sprite sprite;
    SDL_FRect background;
    SDL_FRect floor;

};

