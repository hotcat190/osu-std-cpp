#pragma once


#include "position.h"
#include "hit_type.h"

#include <SDL_timer.h>

class Game;
class Position;

class HitEffect
{
public:
    HitEffect(Game& _game, Position _pos, HIT_TYPE _hit_type, Uint32 _time_appeared);
    ~HitEffect();

    Game& game;

    Position position;
    HIT_TYPE hit_type;

    Uint32 time_appeared;
    Uint32 time_alive;

    const int width = 50;
    const int height = 125;

    void update();
    void render();

};
