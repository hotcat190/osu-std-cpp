#include "approach_circle.h"

#include <iostream>

ApproachCircle::ApproachCircle()
{}


ApproachCircle::~ApproachCircle()
{}

void ApproachCircle::render(SDL_Renderer* ren, Uint32 time_elapsed)
{
    time_alive = time_elapsed - time_appear;
    if (time_alive > AR_scaled)
        return;
    radius -= (AC_scale-1)*CS_scaled*time_alive/AR_scaled;
    std::cout << radius << std::endl;
    std::cout << time_alive << std::endl;
    SDL_Rect draw_area = {position.x - radius/2, position.y - radius/2, radius, radius};
    SDL_RenderCopy(ren, texture, NULL, &draw_area);
}
