#pragma once

#include "position.h"
#include "hit_type.h"

#include <SDL_timer.h>

class Game;

enum HIT_OBJECT_TYPE {
    HIT_CIRCLE = 0x1,
    SLIDER = 0x2,
    NEW_COMBO = 0x4,
    SPINNER = 0x8
};

class HitObject
{
public:
    virtual ~HitObject(){}

    Position position;
    HIT_TYPE hit_type;

    Uint32 time_to_hit;
    Uint32 time_to_appear;

    int combo;

    virtual void handleMotion() = 0;
    virtual void handleClick() = 0;
    virtual void update() = 0;
    virtual void render() = 0;

    bool isHit() {return hit;}
    bool isMiss() {return miss;}

    virtual HIT_OBJECT_TYPE getHitObjectType() = 0;

    bool hit = false;
    bool miss = false;
    bool appeared = false;
private:
    virtual bool InBound() = 0;
};
