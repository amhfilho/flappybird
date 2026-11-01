//
// Created by (Admin) Antonio Filho on 31/10/2026.
//

#include "Sprite.h"
#include <SDL3_image/SDL_image.h>

#include "TextureLoader.h"

Sprite::Sprite(const char* relativePath) :
    texture(nullptr),
    relativePath(relativePath) {
}

// The texture is owned by TextureLoader's cache.
Sprite::~Sprite() = default;

SDL_Texture *Sprite::GetTexture(SDL_Renderer* renderer) {
    if (texture == nullptr) {
        texture = TextureLoader::LoadTexture(renderer, relativePath);
    }
    return texture;
}
