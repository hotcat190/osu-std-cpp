#include "spinner.h"
#include "game.h"

#include <cmath>

#include <SDL.h>

Spinner::Spinner(Game& _game)
    : game(_game)
{
    position.x = game.gWidth/2;
    position.y = game.gHeight/2;
}

Spinner::~Spinner()
{}

void Spinner::handleMotion()
{
    angle = 90 + std::atan2(double(game.cursor.position.y-game.gHeight/2),double(game.cursor.position.x-game.gWidth/2))*180/3.14;
    std::cout << angle << std::endl;
}

void Spinner::handleClick()
{

}

void Spinner::update()
{
    if (game.time_elapsed > time_to_hit)
    hit = true;
}

void Spinner::render()
{
    SDL_Rect dst = {position.x-150, position.x-225, 300, 300};
    SDL_RenderCopyEx(game.gRenderer, game.spinner_circle, nullptr, &dst, angle, nullptr, SDL_FLIP_NONE);
}
