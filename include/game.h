#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>
#include <iostream>
#include <list>

#include "circle.h"
#include "cursor.h"

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

private:
    SDL_Window*   gWindow;
    SDL_Renderer* gRenderer;
    _TTF_Font*    gFont;
    int           gWidth, gHeight;
    bool          running;
    Uint8         sdl_flags;

    std::list<Circle> circles;
    Cursor cursor;

    void init();
    void handleEvents();
    void update();
    void render();
    void clean();
    void log(std::ostream& os, const std::string &msg, bool fatal);
};
