#pragma once

#include "position.h"
#include <SDL.h>

class ApproachCircle
{
public:
    ApproachCircle();
    ~ApproachCircle();

    void render(SDL_Renderer* ren);

    SDL_Texture* texture;
    Uint32 time_appear;

    Position position;

private:
    Uint32 time_alive;
    int radius = 3*128-18;
};
