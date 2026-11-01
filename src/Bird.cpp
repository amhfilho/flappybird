//
// Created by Filho, Antonio on 29/10/2026.
//

#include "Bird.h"

#include <SDL3/SDL_render.h>
#include "Colors.h"
#include "Constants.h"
#include "Game.h"
#include "TextureLoader.h"

Bird::Bird(Game& game, Vector2 position) :
    game(game),
    position(position),
    velocity(0.0f),
    gravity(250.0f) {

}

void Bird::Update(float deltaTime) {
    velocity = velocity + gravity * deltaTime;
    position.y = position.y + velocity * deltaTime;
    SDL_FRect rect = GetRect();
    if (rect.y + rect.h >= Constants::Window::Height - Constants::Sprites::Floor::Height) {
        position.y = Constants::Window::Height - Constants::Sprites::Floor::Height - rect.h;
    }
}

void Bird::Flap() {
    velocity = -100.0f;
}

void Bird::Draw(SDL_Renderer &renderer) {
    SDL_Texture* texture = TextureLoader::LoadTexture(&renderer, "Graphics/sprites.png");
    SDL_FRect rect = {
        Constants::Bird::Sprite::X,
        Constants::Bird::Sprite::Y,
        Constants::Bird::Sprite::Width,
        Constants::Bird::Sprite::Height };
    SDL_FRect dest = GetRect();
    SDL_RenderTexture(&renderer, texture, &rect, &dest);

    // SDL_FRect rect = GetRect();
    // SDL_SetRenderDrawColor(&renderer, white.r, white.g, white.b, white.a);
    // SDL_RenderFillRect(&renderer, &rect);
}

SDL_FRect Bird::GetRect() {
    return SDL_FRect {position.x, position.y, Constants::Bird::Sprite::Width, Constants::Bird::Sprite::Height };
}
