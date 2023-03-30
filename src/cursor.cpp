#include "cursor.h"

Cursor::Cursor()
    : expand(false)
{
    SDL_GetMouseState(&position.x, &position.y);
    cursorRect.x = position.x - 55/2;
    cursorRect.y = position.y - 55/2;
}

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
    SDL_ShowCursor(false);
}

void Cursor::render(SDL_Renderer* ren)
{
    SDL_RenderCopy(ren, cursorTexture, nullptr, &cursorRect);
}

Cursor::~Cursor()
{}
