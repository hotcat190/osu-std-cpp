#include "ur_bar.h"
#include "game.h"
#include "hit_object.h"
#include "hit_window.h"

#include <SDL.h>

UR_Bar::UR_Bar(Game& _game)
    : game(_game),
      hit50_early  {game.gWidth/2-120, game.gHeight-18, 40 , 5},
      hit100_early {game.gWidth/2-80 , game.gHeight-18, 40 , 5},
      hit300       {game.gWidth/2-40 , game.gHeight-18, 80 , 5},
      hit100_late  {game.gWidth/2+40 , game.gHeight-18, 40 , 5},
      hit50_late   {game.gWidth/2+80 , game.gHeight-18, 40 , 5},
      middle       {game.gWidth/2-2  , game.gHeight-28, 4  , 25}
{}

UR_Bar::~UR_Bar()
{}

void UR_Bar::update()
{
    int hit_time = game.time_elapsed - game.render_stack.back()->time_to_hit;
    int pos = game.gWidth/2 + hit_time;
    SDL_Rect hit_rect = {pos, game.gHeight-28, 4, 25};
    UR_HIT _hit{hit_rect, game.time_elapsed, 0};
    hits.push_back(_hit);

    for (auto hit : hits)
    {
        hit.time_alive = game.time_elapsed - hit.time_appeared;
        if (hit.time_alive > 3000)
            hits.pop_front();
    }
}

void UR_Bar::render()
{
    SDL_SetRenderDrawColor(game.gRenderer, 255, 215, 0, 255);
    SDL_RenderFillRect(game.gRenderer, &hit50_early);
    SDL_RenderFillRect(game.gRenderer, &hit50_late);

    SDL_SetRenderDrawColor(game.gRenderer, 0, 255, 0, 255);
    SDL_RenderFillRect(game.gRenderer, &hit100_early);
    SDL_RenderFillRect(game.gRenderer, &hit100_late);

    SDL_SetRenderDrawColor(game.gRenderer, 25, 255, 255, 255);
    SDL_RenderFillRect(game.gRenderer, &hit300);

    for (auto hit : hits)
    {
        SDL_Point point = {hit.hit_rect.x, game.gHeight-18};

        if (SDL_PointInRect(&point, &hit50_early) || SDL_PointInRect(&point, &hit50_late))
            SDL_SetRenderDrawColor(game.gRenderer, 255, 225, 50, 255);
        else if (SDL_PointInRect(&point, &hit100_early) || SDL_PointInRect(&point, &hit100_late))
            SDL_SetRenderDrawColor(game.gRenderer, 50, 255, 50, 255);
        else if (SDL_PointInRect(&point, &hit300))
            SDL_SetRenderDrawColor(game.gRenderer, 100, 255, 255, 255);

        SDL_RenderFillRect(game.gRenderer, &hit.hit_rect);
    }

    SDL_SetRenderDrawColor(game.gRenderer, 255, 255, 255, 255);
    SDL_RenderFillRect(game.gRenderer, &middle);

    SDL_SetRenderDrawColor(game.gRenderer, 0, 0, 0, 255);
}
