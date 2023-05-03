#pragma once

#include "hit_type.h"

class Game;

class ScoreManager
{
public:
    ScoreManager(Game& _game);
    ~ScoreManager();

    Game& game;

    unsigned long long score;
    int combo;
    int max_combo;

    int nums_hit300;
    int nums_hit300g;
    int nums_hit100;
    int nums_hit100k;
    int nums_hit50;
    int nums_hit0;

    float accuracy;

    void update(HIT_TYPE hit_type);
};
