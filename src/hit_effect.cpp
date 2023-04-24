#include "hit_effect.h"

#include "game.h"

HitEffect::HitEffect(Game& _game, Position _pos, HIT_TYPE _hit_type, Uint32 _time_appeared)
    : game(_game),
      position(_pos),
      hit_type(_hit_type),
      time_appeared(_time_appeared),
      time_alive(0)
{}

HitEffect::~HitEffect()
{}

void HitEffect::update()
{
    time_alive = SDL_GetTicks() - time_appeared;
}

void HitEffect::render()
{
    SDL_Rect dst = {position.x - width/2, position.y - height/4, width, height};
    if (hit_type == HIT300)
    {
        SDL_RenderCopy(game.gRenderer, game.hit300, nullptr, &dst);
    }
    if (hit_type == HIT100)
    {
        SDL_RenderCopy(game.gRenderer, game.hit100, nullptr, &dst);
    }
    else if (hit_type == HIT50)
    {
        SDL_RenderCopy(game.gRenderer, game.hit50, nullptr, &dst);
    }
    else if (hit_type == HIT0)
    {
        SDL_RenderCopy(game.gRenderer, game.hit0, nullptr, &dst);
    }
}
