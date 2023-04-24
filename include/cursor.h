#pragma once

#include "position.h"
#include <SDL.h>

class Game;

class Cursor
{
public:
    Cursor(Game& _game);
    ~Cursor();

    Game& game;

    Position position;
    SDL_Rect cursorRect = {0, 0, 55, 55};
    SDL_Texture* cursorTexture;

    void handleMotion();
    void handleClick();
    void update();
    void render();
private:
    bool expand;
};
