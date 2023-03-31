#include "circle.h"
#include <cmath>
#include <iostream>

Circle::Circle()
    : position(400, 300),
      hitcircle(nullptr),
      hitcircleoverlay(nullptr),
      hit(false),
      miss(false),
      radius(CS_scaled),
      time_to_hit(0),
      time_to_appear(0),
      approachcircle()
{
    for (int i = 0; i < 10; i++)
    {
        defaults[i] = nullptr;
    }
}

Circle::~Circle()
{

}

bool Circle::InBound()
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    return (std::sqrt((x-position.x)*(x-position.x)+(y-position.y)*(y-position.y)) <= radius/2);
}

void Circle::handleClick(Uint32 time_elapsed)
{
    if (time_elapsed < time_to_appear)
        return;
    else if (InBound())
    {
        //register hit
        hit = true;
    }
}

void Circle::update(Uint32 time_elapsed)
{
    int HIT_WINDOW_50 = 120;
    if (!hit && time_elapsed > time_to_hit + HIT_WINDOW_50)
    {
//        render fade out effect
        std::cout << time_elapsed << std::endl;
        miss = true;
    }
}

void Circle::render(SDL_Renderer* ren, Uint32 time_elapsed)
{
//    if (hit || miss)
//        return;
    if (time_elapsed < time_to_appear)
        return;
    SDL_Rect dst = {position.x - radius/2, position.y - radius/2, radius, radius};
    SDL_RenderCopy(ren, hitcircle, NULL, &dst);
    SDL_RenderCopy(ren, defaults[1], NULL, &dst);

    approachcircle.render(ren, time_elapsed);
}



