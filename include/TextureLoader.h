//
// Created by (Admin) Antonio Filho on 31/10/2026.
//
#pragma once
#include <string>
#include <unordered_map>

struct SDL_Renderer;
struct SDL_Texture;

class TextureLoader {
public:
    static SDL_Texture* LoadTexture(SDL_Renderer* renderer, const char* relativePath);
    static void Clear();

private:
    static std::unordered_map<std::string, SDL_Texture*> cache;
};
