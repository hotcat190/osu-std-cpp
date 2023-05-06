#pragma once

#include <deque>
#include <SDL.h>

struct UR_HIT
{
    SDL_Rect hit_rect;
    Uint32 time_appeared;
    Uint32 time_alive;
};

class Game;

class UR_Bar
{
public:
    UR_Bar(Game& _game);
    ~UR_Bar();

    Game& game;

    SDL_Rect hit50_early;
    SDL_Rect hit100_early;
    SDL_Rect hit300;
    SDL_Rect hit100_late;
    SDL_Rect hit50_late;
    SDL_Rect middle;

    std::deque<UR_HIT> hits;

    void update();
    void render();
private:

};
