//
// Created by Antonio Mario Henriques Filho on 01/11/2026.
//

#include "UpperPipe.h"

#include <SDL3/SDL_render.h>

#include "Constants.h"
#include "TextureLoader.h"

UpperPipe::UpperPipe(Vector2 position) :
    position(position) {

};

void UpperPipe::Draw(SDL_Renderer* renderer) {
    SDL_Texture* texture = TextureLoader::LoadTexture(renderer, Constants::Sprites::TileSetPath);
    SDL_FRect srcRect = {
        Constants::Sprites::UpperPipe::X,
        Constants::Sprites::UpperPipe::Y,
        Constants::Sprites::UpperPipe::Width,
        Constants::Sprites::UpperPipe::Height
    };
    SDL_FRect dstRect = GetRect();
    SDL_RenderTexture(renderer, texture, &srcRect, &dstRect);
}

SDL_FRect UpperPipe::GetRect() {
    return {
        position.x,
        position.y,
        Constants::Sprites::UpperPipe::Width,
        Constants::Sprites::UpperPipe::Height
    };
}


