//
// Created by (Admin) Antonio Filho on 31/10/2026.
//
#pragma once
#include <SDL3/SDL_render.h>

class TextureLoader {
public:
    static SDL_Texture* LoadTexture(SDL_Renderer* renderer, const char* relativePath);
};
