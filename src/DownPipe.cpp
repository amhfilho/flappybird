//
// Created by Antonio Mario Henriques Filho on 01/11/2026.
//

#include "DownPipe.h"

#include <SDL3/SDL_render.h>
#include "Constants.h"
#include "TextureLoader.h"

DownPipe::DownPipe(Vector2 position) :
    position(position) {

    };

void DownPipe::Draw(SDL_Renderer* renderer) {
    SDL_Texture* texture = TextureLoader::LoadTexture(renderer, Constants::Sprites::TileSetPath);
    SDL_FRect srcRect = {
        Constants::Sprites::DownPipe::X,
        Constants::Sprites::DownPipe::Y,
        Constants::Sprites::DownPipe::Width,
        Constants::Sprites::DownPipe::Height
    };
    SDL_FRect dstRect = GetRect();
    SDL_RenderTexture(renderer, texture, &srcRect, &dstRect);
}

SDL_FRect DownPipe::GetRect() {
    return {
        position.x,
        position.y,
        Constants::Sprites::DownPipe::Width,
        Constants::Sprites::DownPipe::Height
    };
}
