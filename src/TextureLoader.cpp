//
// Created by (Admin) Antonio Filho on 31/10/2026.
//

#include "TextureLoader.h"
#include "AssetLoader.h"
#include <SDL3_image/SDL_image.h>

std::unordered_map<std::string, SDL_Texture*> TextureLoader::cache;

SDL_Texture* TextureLoader::LoadTexture(SDL_Renderer* renderer, const char* relativePath) {
    auto it = cache.find(relativePath);
    if (it != cache.end()) {
        return it->second;
    }

    SDL_Log("Loading texture: %s", relativePath);
    std::string path = AssetLoader::GetAssetPath(relativePath);
    SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
    if (texture == nullptr) {
        SDL_Log("Error loading texture: %s", SDL_GetError());
        return nullptr;
    }
    cache[relativePath] = texture;
    return texture;
}

void TextureLoader::Clear() {
    for (auto& entry : cache) {
        SDL_DestroyTexture(entry.second);
    }
    cache.clear();
}
