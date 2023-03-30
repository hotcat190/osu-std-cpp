#include "circle.h"
#include <cmath>

Circle::Circle()
    : position(400, 300),
      hitcircle(nullptr),
      hitcircleoverlay(nullptr),
      hit(false),
      radius(128)
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

void Circle::handleClick()
{
    if (InBound())
    {
        //register hit
        hit = true;
    }
}

void Circle::update()
{
    if (hit)
    {
        //render fade out effect

    }
}

void Circle::render(SDL_Renderer* ren)
{
    if (hit)
        return;
    SDL_Rect dst = {position.x - radius/2, position.y - radius/2, radius, radius};
    SDL_RenderCopy(ren, hitcircle, NULL, &dst);
    SDL_RenderCopy(ren, defaults[1], NULL, &dst);
}



