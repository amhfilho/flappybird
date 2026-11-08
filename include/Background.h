//
// Created by (Admin) Antonio Filho on 31/10/2026.
//

#pragma once
#include <SDL3/SDL_rect.h>

#include "Sprite.h"

class Background {
public:
    Background();
    ~Background() = default;

    void Draw(SDL_Renderer* renderer);

private:
    Sprite sprite;
    SDL_FRect background;
};

