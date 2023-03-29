#pragma once

#include "position.h"
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

    bool hit;

    void handleClick();
    void update();
    void render(SDL_Renderer* ren);

private:
};
