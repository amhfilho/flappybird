//
// Created by (Admin) Antonio Filho on 31/10/2026.
//

#pragma once
struct SDL_Renderer;
struct SDL_Texture;

class Sprite {
public:
    Sprite(const char* relativePath);
    virtual ~Sprite();

    SDL_Texture* GetTexture(SDL_Renderer* renderer);

private:
    const char* relativePath;
    SDL_Texture* texture;

};

