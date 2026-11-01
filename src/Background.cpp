//
// Created by (Admin) Antonio Filho on 31/10/2026.
//
#include "Background.h"
#include "Constants.h"
#include "TextureLoader.h"

Background::Background() :
    sprite(Constants::Sprites::TileSetPath),
    background{},
    floor{} {

    background = {
        Constants::Sprites::Background::X,
        Constants::Sprites::Background::Y,
        Constants::Sprites::Background::Width,
        Constants::Sprites::Background::Height
    };

    floor = {
        Constants::Sprites::Floor::X,
        Constants::Sprites::Floor::Y,
        Constants::Sprites::Floor::Width,
        Constants::Sprites::Floor::Height
    };
}

void Background::Update(float deltaTime) {
    
}

void Background::Draw(SDL_Renderer* renderer) {
    SDL_Texture *texture = sprite.GetTexture(renderer);

    for (float x = 0.0f;
        x + background.w < Constants::Window::Width ;
        x += background.w - 2) {
        SDL_FRect dest = {
            x,
            Constants::Window::Height - background.h,
            Constants::Sprites::Background::Width,
            Constants::Sprites::Background::Height
        };
        SDL_RenderTexture(renderer, texture, &background, &dest);
    }

    for (float x = 0.0f;
        x < Constants::Window::Width ;
        x += floor.w - 2) {
        SDL_FRect dest = {
            x,
            Constants::Window::Height - floor.h,
            floor.w,
            floor.h
        };
        SDL_RenderTexture(renderer, texture, &floor, &dest);
    }
}

float Background::GetFloorY() {
    return floor.y;
}