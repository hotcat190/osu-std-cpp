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

    Position position;

    void handleMotion() override;
    void handleClick() override;
    void update() override;
    void render() override;

    bool isHit() override;
    bool isMiss() override;
};
