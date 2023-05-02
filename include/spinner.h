#pragma once

#include "hit_object.h"
#include "position.h"

class Game;

class Spinner : public HitObject
{
public:
    Spinner(Game& _game);
    ~Spinner();

    Game& game;

    void handleMotion() override;
    void handleClick() override;
    void handleRelease() override;
    void update() override;
    void render() override;

    HIT_OBJECT_TYPE getHitObjectType() override {return SPINNER;}
private:
    bool InBound() override {return true;}

    double calculateDeltaAngle();

    double angle;
    double delta_angle;

    Position prev_cursor_pos;
    Position curr_cursor_pos;

    int spinner_gauge;
    double velocity;
    double acceleration;
    bool spinning;

    const double ACCELERATION_MAX = 1;
    const double VELOCITY_MAX = 2;
};
