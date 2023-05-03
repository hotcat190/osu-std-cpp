#include "score_manager.h"
#include "game.h"

ScoreManager::ScoreManager(Game& _game)
    : game(_game),
      score(0),
      combo(0),
      max_combo(0),
      nums_hit300(0),
      nums_hit300g(0),
      nums_hit100(0),
      nums_hit100k(0),
      nums_hit50(0),
      nums_hit0(0),
      accuracy(100.00)
{}

ScoreManager::~ScoreManager()
{}

void ScoreManager::update(HIT_TYPE hit_type)
{
    switch (hit_type)
    {
    case HIT300:
        nums_hit300++;
        combo++;
        score += 300*combo;
        break;

    case HIT100:
        nums_hit100++;
        combo++;
        score += 100*combo;
        break;

    case HIT50:
        nums_hit50++;
        combo++;
        score += 50*combo;
        break;

    case HIT0:
        nums_hit0++;
        combo = 0;
        break;
    }

    if (combo > max_combo)
        max_combo = combo;

    accuracy = float(300*nums_hit300 + 100*nums_hit100 + 50*nums_hit50)
             / float(3*(nums_hit300 + nums_hit100 + nums_hit50 + nums_hit0));
}
