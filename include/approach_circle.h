#pragma once

#include <SDL.h>

#include "position.h"
#include "difficulty_modifiers.h"

class ApproachCircle
{
public:
    ApproachCircle();
    ~ApproachCircle();

    void render(SDL_Renderer* ren, Uint32 time_elapsed);

    SDL_Texture* texture;
    Uint32 time_appear;

    Position position;

private:
    Uint32 time_alive;
    const float AC_scale = 5/2;
    int radius = AC_scale*CS_scaled+6;
};
