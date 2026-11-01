//
// Created by (Admin) Antonio Filho on 31/10/2026.
//

#pragma once
#include <SDL3/SDL_render.h>

class Sprite {
public:
    Sprite(const char* relativePath);
    virtual ~Sprite();

    SDL_Texture* GetTexture(SDL_Renderer* renderer);

private:
    const char* relativePath;
    SDL_Texture* texture;

};

