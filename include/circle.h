#pragma once

#include "position.h"
#include "difficulty_modifiers.h"

#include "approach_circle.h"

#include "hit_type.h"
#include "hit_object.h"

class Game;

class Circle : public HitObject
{
public:
    Circle(Game& _game);
    ~Circle();

    Game& game;

    ApproachCircle  approachcircle;

    bool isHit() override {return hit;}
    bool isMiss() override {return miss;}

    void handleClick() override;
    void handleMotion() override;
    void update() override;
    void render() override;

    HIT_OBJECT_TYPE getHitObjectType() override {return HIT_CIRCLE;}

private:
    int radius;
    bool InBound() override;
    bool hit;
    bool miss;
    bool appeared;
};
