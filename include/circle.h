#pragma once

#include "position.h"
#include "approach_circle.h"
#include "difficulty_modifiers.h"

#include <SDL.h>
#include <SDL_mixer.h>

class Circle
{
public:
    Circle();
    ~Circle();

    SDL_Texture* getHitCircleTexture() {return hitcircle;}

    Position        position;
    SDL_Texture*    hitcircle;
    SDL_Texture*    hitcircleoverlay;
    SDL_Texture*    defaults[10];
    ApproachCircle  approachcircle;

    bool isHit() {return hit;}
    bool isMiss() {return miss;}

    void handleClick(Uint32 time_elapsed);
    void update(Uint32 time_elapsed);
    void render(SDL_Renderer* ren, Uint32 time_elapsed);

    Uint32 time_to_hit;
    Uint32 time_to_appear;

private:
    int radius;
    bool InBound();
    bool hit;
    bool miss;
    bool appeared;
};
