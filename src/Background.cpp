//
// Created by (Admin) Antonio Filho on 31/10/2026.
//
#include "Background.h"

#include <SDL3/SDL_render.h>
#include "Constants.h"
#include "TextureLoader.h"

Background::Background() :
    sprite(Constants::Sprites::TileSetPath),
    background{} {

    background = {
        Constants::Sprites::Background::X,
        Constants::Sprites::Background::Y,
        Constants::Sprites::Background::Width,
        Constants::Sprites::Background::Height
    };
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


}