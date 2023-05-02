#include "spinner.h"
#include "game.h"

#include <cmath>

#include <SDL.h>

Spinner::Spinner(Game& _game)
    : game(_game),
      velocity(0),
      acceleration(0),
      angle(0),
      delta_angle(0)
{
    position = {game.gWidth/2, game.gHeight/2};
}

Spinner::~Spinner()
{}

void Spinner::handleMotion()
{
    curr_cursor_pos = game.cursor.position;
}

void Spinner::handleClick()
{
    spinning = true;
}

void Spinner::handleRelease()
{
    spinning = false;
}

double Spinner::calculateDeltaAngle()
{
    return 180/3.14 * (std::atan2(curr_cursor_pos.y - position.y, curr_cursor_pos.x - position.x) -
                         std::atan2(prev_cursor_pos.y - position.y, prev_cursor_pos.x - position.x));
}

void Spinner::update()
{
    if (game.time_elapsed > time_to_appear && spinning)
    {
        delta_angle = calculateDeltaAngle();
        std::cout << delta_angle << std::endl;

        double angle_friction = 0;
        if (velocity != 0)
        {
            angle_friction = -velocity/200;
        }

        acceleration = delta_angle + angle_friction;
        if      (acceleration >  ACCELERATION_MAX) acceleration =  ACCELERATION_MAX;
        else if (acceleration < -ACCELERATION_MAX) acceleration = -ACCELERATION_MAX;

        velocity += acceleration * game.delta_time;
        if      (velocity >  VELOCITY_MAX) velocity =  VELOCITY_MAX;
        else if (velocity < -VELOCITY_MAX) velocity = -VELOCITY_MAX;

        angle += velocity * game.delta_time;
    }
    prev_cursor_pos = curr_cursor_pos;

    spinner_gauge = std::abs(angle)/5;
    if (spinner_gauge < 50) hit_type = HIT0;
    else if (spinner_gauge < 100) hit_type = HIT50;
    else if (spinner_gauge < 300) hit_type = HIT100;
    else hit_type = HIT300;

    if (game.time_elapsed > time_to_hit)
    {
        if (hit_type == HIT0) miss = true;
        else hit = true;
    }
}

void Spinner::render()
{
    SDL_Rect dst = {position.x-150, position.y-150, 300, 300};
    SDL_RenderCopyEx(game.gRenderer, game.spinner_circle, nullptr, &dst, angle, nullptr, SDL_FLIP_NONE);
}
