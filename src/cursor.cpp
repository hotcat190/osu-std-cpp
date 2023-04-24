#include "cursor.h"
#include "game.h"

Cursor::Cursor(Game& _game)
    : game(_game),
      expand(false)
{
    SDL_GetMouseState(&position.x, &position.y);
    cursorRect.x = position.x - 55/2;
    cursorRect.y = position.y - 55/2;
}

Cursor::~Cursor()
{}

void Cursor::handleMotion()
{
    SDL_GetMouseState(&position.x, &position.y);
}

void Cursor::handleClick()
{
    if (expand)
    {
        //play expand effect
    }
}

void Cursor::update()
{
    cursorRect.x = position.x - 55/2;
    cursorRect.y = position.y - 55/2;
}

void Cursor::render()
{
    SDL_RenderCopy(game.gRenderer, cursorTexture, nullptr, &cursorRect);
}


