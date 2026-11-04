//
// Created by Antonio Mario Henriques Filho on 01/11/2026.
//
#pragma once
#include "DownPipe.h"
#include "UpperPipe.h"
#include "Vector2.h"

class PipesPair {
public:
    PipesPair(Vector2 position);
    ~PipesPair() = default;

    void Update(float deltTtime);
    void Draw(SDL_Renderer* renderer);

    const UpperPipe& GetUpperPipe() const { return upperPipe; }
    const DownPipe& GetDownPipe() const { return downPipe; }


private:
    static float RandomFloat(float min, float max);

    Vector2 position;
    UpperPipe upperPipe;
    DownPipe downPipe;
};
