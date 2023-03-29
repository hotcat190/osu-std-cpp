#include "circle.h"
#include <cmath>

Circle::Circle()
    : position(1280/2, 720/2),
      hitcircle(nullptr),
      hitcircleoverlay(nullptr),
      hit(false)
{
    for (int i = 0; i < 10; i++)
    {
        defaults[i] = nullptr;
    }
}

Circle::~Circle()
{

}

void Circle::handleClick()
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    if (std::sqrt((x-position.x)*(x-position.x)+(y-position.y)*(y-position.y)) <= 128/2) //xy is inside circle
    {
        //register hit
        hit = true;
    }
}

void Circle::update()
{
    if (hit)
    {
        //play sound effect

    }
}

void Circle::render(SDL_Renderer* ren)
{
    if (hit)
        return;
    SDL_Rect dst = {position.x - 128/2, position.y - 128/2, 128, 128};
    SDL_RenderCopy(ren, hitcircle, NULL, &dst);
    SDL_RenderCopy(ren, defaults[1], NULL, &dst);
}



