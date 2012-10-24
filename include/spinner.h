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
    void update() override;
    void render() override;

    HIT_OBJECT_TYPE getHitObjectType() override {return SPINNER;}
private:
    bool InBound() override {return true;}
    double angle;

};
