#pragma once

#include <SDL.h>

#include "position.h"
#include "difficulty_modifiers.h"

class Game;

class ApproachCircle
{
public:
    ApproachCircle(Game& _game);
    ~ApproachCircle();

    Game& game;

    void render();


    Uint32 time_appear;

    Position position;

private:
    Uint32 time_alive;
    const float AC_scale = 6/2;
    int radius = AC_scale*CS_scaled+6;
};
