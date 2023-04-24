#include "spinner.h"
#include "game.h"

#include <SDL.h>

Spinner::Spinner(Game& _game)
    : game(_game)
{}

Spinner::~Spinner()
{}

void Spinner::handleMotion()
{

}

void Spinner::handleClick()
{

}

bool Spinner::isHit()
{

}

bool Spinner::isMiss()
{

}

void Spinner::update()
{

}

void Spinner::render()
{
    SDL_Rect dst = {game.gWidth/2, game.gHeight/2, 300, 300};
    SDL_RenderCopy(game.gRenderer, game.spinner_circle, nullptr, &dst);
}
