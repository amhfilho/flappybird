//
// Created by Antonio Mario Henriques Filho on 01/11/2026.
//

#include "Floor.h"

#include <SDL3/SDL_render.h>
#include "Constants.h"
#include "Sprite.h"

Floor::Floor() :
    rect{},
    scrollOffset(0.0f) {

    rect = {
        Constants::Sprites::Floor::X,
        Constants::Sprites::Floor::Y,
        Constants::Sprites::Floor::Width,
        Constants::Sprites::Floor::Height
    };
};

void Floor::Update(float deltaTime) {
    // Tiles are drawn every `tileStride` pixels, so the pattern repeats after one stride.
    scrollOffset = SDL_fmodf(scrollOffset + ScrollSpeed * deltaTime, TileStride());
}

void Floor::Draw(SDL_Renderer* renderer, Sprite& tileSet) {
    SDL_Texture *texture = tileSet.GetTexture(renderer);

    for (float x = -scrollOffset;
        x < Constants::Window::Width ;
        x += TileStride()) {
        SDL_FRect dest = {
            x,
            Constants::Window::Height - rect.h,
            rect.w,
            rect.h
        };
        SDL_RenderTexture(renderer, texture, &rect, &dest);
    }
}

float Floor::GetFloorY() {
    return rect.y;
}

