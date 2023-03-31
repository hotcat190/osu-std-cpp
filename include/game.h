#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <string>
#include <iostream>
#include <deque>

#include "circle.h"
#include "cursor.h"
#include "beatmap_manager.h"

class Game
{
public:
    Game();
    Game(const Game&) = delete;
    Game(Game&&) = delete;

    Game& operator=(const Game&) = delete;
    Game& operator=(Game&&) = delete;

    ~Game();

    void start();

    Mix_Chunk* hitnormal;
    Mix_Music* music;
    int effectVolume;
    int musicVolume;
    int masterVolume;
    int gWidth, gHeight;
    Uint32 time_elapsed;

    std::deque<Circle> circles;
    Cursor cursor;

private:
    SDL_Window*   gWindow;
    SDL_Renderer* gRenderer;
    _TTF_Font*    gFont;
    bool          running;
    Uint8         sdl_flags;

    Uint32 init_time;

    void init();
    void handleEvents();
    void update();
    void render();
    void clean();
    void log(std::ostream& os, const std::string &msg, bool fatal) const;
};
