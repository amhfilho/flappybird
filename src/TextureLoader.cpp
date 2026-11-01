//
// Created by (Admin) Antonio Filho on 31/10/2026.
//

#include "TextureLoader.h"
#include "AssetLoader.h"
#include <SDL3_image/SDL_image.h>

SDL_Texture* TextureLoader::LoadTexture(SDL_Renderer* renderer, const char* relativePath) {
    std::string path = AssetLoader::GetAssetPath(relativePath);
    SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
    if (texture == nullptr) {
        SDL_Log("Error loading texture: %s", SDL_GetError());
    }
    return texture;
}
