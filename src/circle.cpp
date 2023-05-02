#include "circle.h"
#include "game.h"
#include "texture_manager.h"

#include <cmath>

enum {
    HIT_WINDOW_50 = 120,
    HIT_WINDOW_100 = 60,
    HIT_WINDOW_300 = 30
} HIT_WINDOW;

Circle::Circle(Game& _game)
    : game(_game),
      radius(CS_scaled),
      approachcircle(_game)
{}

Circle::~Circle()
{}

void Circle::handleMotion()
{}

bool Circle::InBound()
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    return (std::sqrt((x-position.x)*(x-position.x)+(y-position.y)*(y-position.y)) <= radius/2);
}

void Circle::handleClick()
{
    if (game.time_elapsed < time_to_appear)
        return;
    else if (isHit() || isMiss())
        return;
    else if (InBound())
    {
        //register hit
        if (game.time_elapsed > time_to_appear && game.time_elapsed < time_to_hit - HIT_WINDOW_50)
        {
            HitObject::hit_type = HIT0;
            miss = true;
        }
        else if ((game.time_elapsed > time_to_hit - HIT_WINDOW_50 && game.time_elapsed < time_to_hit - HIT_WINDOW_100) ||
                 (game.time_elapsed > time_to_hit + HIT_WINDOW_100 && game.time_elapsed < time_to_hit + HIT_WINDOW_50)
                 )
        {
            HitObject::hit_type = HIT50;
            hit = true;
        }
        else if ((game.time_elapsed > time_to_hit - HIT_WINDOW_100 && game.time_elapsed < time_to_hit - HIT_WINDOW_300) ||
                 (game.time_elapsed > time_to_hit + HIT_WINDOW_300 && game.time_elapsed < time_to_hit + HIT_WINDOW_100)
                 )
        {
            HitObject::hit_type = HIT100;
            hit = true;
        }
        else if ((game.time_elapsed > time_to_hit - HIT_WINDOW_300 && game.time_elapsed < time_to_hit + HIT_WINDOW_300))
        {
            HitObject::hit_type = HIT300;
            hit = true;
        }
    }
}

void Circle::update()
{

    if (!hit && game.time_elapsed > time_to_hit + HIT_WINDOW_50)
    {
//        render fade out effect
        miss = true;
    }
}

void Circle::render()
{
    if (hit || miss)
        return;
    SDL_Rect dst = {position.x - radius/2, position.y - radius/2, radius, radius};
    SDL_RenderCopy(game.gRenderer, game.gTexture->hitcircle, NULL, &dst);
    SDL_RenderCopy(game.gRenderer, game.gTexture->defaults[combo], NULL, &dst);

    approachcircle.render();
}



