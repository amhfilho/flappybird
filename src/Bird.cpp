//
// Created by Filho, Antonio on 29/10/2026.
//

#include "Bird.h"

#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include "Colors.h"
#include "Constants.h"
#include "Game.h"
#include "Sprite.h"
#include "TextureLoader.h"

Bird::Bird(Game& game, Vector2 position) :
    game(game),
    position(position),
    velocity(0.0f),
    gravity(Constants::Sprites::Bird::Gravity),
    currentFrame(0.0f),
    angle(0),
    inFloor(false) {

    spriteRects.push_back({
        Constants::Sprites::Bird::Sprite1::X,
        Constants::Sprites::Bird::Sprite1::Y,
        Constants::Sprites::Bird::Width,
        Constants::Sprites::Bird::Height });
    spriteRects.push_back({
        Constants::Sprites::Bird::Sprite2::X,
        Constants::Sprites::Bird::Sprite2::Y,
        Constants::Sprites::Bird::Width,
        Constants::Sprites::Bird::Height });
    spriteRects.push_back({
        Constants::Sprites::Bird::Sprite3::X,
        Constants::Sprites::Bird::Sprite3::Y,
        Constants::Sprites::Bird::Width,
        Constants::Sprites::Bird::Height });

}

void Bird::Update(float deltaTime) {
    velocity = velocity + gravity * deltaTime;
    position.y = position.y + velocity * deltaTime;
    SDL_FRect rect = GetRect();
    if (rect.y + rect.h >= Constants::Window::Height - Constants::Sprites::Floor::Height) {
        position.y = Constants::Window::Height - Constants::Sprites::Floor::Height - rect.h;
        inFloor = true;
    }

    currentFrame += Constants::Sprites::Bird::AnimationFPS * deltaTime;

    // Wrap current frame if needed
    while (currentFrame >= spriteRects.size()) {
        currentFrame -= spriteRects.size();
    }
}

void Bird::Flap() {
    if (game.GetGameState() != Game::GameState::BirdHurt) {
        velocity = -Constants::Sprites::Bird::FlapSpeed;
    }
}

void Bird::Draw(SDL_Renderer &renderer, Sprite& tileSet) {
    SDL_Texture* texture = tileSet.GetTexture(&renderer);
    SDL_FRect sprite = spriteRects.at(currentFrame);
    SDL_FRect dest = GetRect();
    if (angle < Constants::Sprites::Bird::DownAngle) {
        angle += Constants::Sprites::Bird::DownAngleIncrement;
    }
    if (velocity < 0.0f) {
        if (angle > Constants::Sprites::Bird::UpAngle) {
            angle -= Constants::Sprites::Bird::UpAngleDecrement;
        }
        angle = Constants::Sprites::Bird::UpAngle;
    }

    SDL_RenderTextureRotated(&renderer, texture, &sprite, &dest, angle, nullptr, SDL_FLIP_NONE);
}

void Bird::OnPipeCollide() {
    angle = 90;
    velocity = 500;
}

SDL_FRect Bird::GetRect() const {
    return SDL_FRect {position.x, position.y, Constants::Sprites::Bird::Width, Constants::Sprites::Bird::Height };
}

bool Bird::IsInFloor() const {
    return inFloor;
}

void Bird::Reset() {
    position = Vector2(Constants::Sprites::Bird::StartX, Constants::Sprites::Bird::StartY);
    velocity = 0.0f;
    angle = 0;
    inFloor = false;
    currentFrame = 0.0f;
}
