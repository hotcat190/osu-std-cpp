#include "approach_circle.h"
#include <iostream>
ApproachCircle::ApproachCircle()
{}


ApproachCircle::~ApproachCircle()
{}

void ApproachCircle::render(SDL_Renderer* ren)
{
    time_alive = SDL_GetTicks() - time_appear;
    if (time_alive > 900*2-200)
        return;
    radius -= 128*time_alive/900;
    std::cout << radius << std::endl;
    std::cout << SDL_GetTicks() << std::endl;
    SDL_Rect draw_area = {position.x - radius/2, position.y - radius/2, radius, radius};
    SDL_RenderCopy(ren, texture, NULL, &draw_area);
}
