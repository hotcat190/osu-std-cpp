#include "approach_circle.h"
#include "game.h"

ApproachCircle::ApproachCircle(Game& _game)
    : game(_game)
{}


ApproachCircle::~ApproachCircle()
{}

void ApproachCircle::render()
{
    time_alive = game.time_elapsed - time_appear;

    if (time_alive > AR_scaled)
        return;

    int shrink_radius = (radius-CS_scaled)*(1.f-float(time_alive)/float(AR_scaled));
    int render_radius = CS_scaled + shrink_radius;

    SDL_Rect dst = {position.x - render_radius/2, position.y - render_radius/2, render_radius, render_radius};
    SDL_RenderCopy(game.gRenderer, game.approachcircle_texture, NULL, &dst);
}
