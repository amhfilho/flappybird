//
// Created by Antonio Mario Henriques Filho on 01/11/2026.
//
#pragma once
#include <SDL3/SDL_rect.h>

#include "Sprite.h"

class Floor {
public:
    Floor();
    ~Floor() = default;

    void Update(float deltaTime);
    void Draw(SDL_Renderer* renderer);

    float GetFloorY();

private:
    static constexpr float ScrollSpeed = 100.0f; // pixels per second, towards the left

    // Distance between tiles; 2px overlap hides seams between them.
    float TileStride() const { return rect.w - 2.0f; }

    Sprite sprite;
    SDL_FRect rect;     // source rectangle in the atlas, never modified after construction
    float scrollOffset;
};

