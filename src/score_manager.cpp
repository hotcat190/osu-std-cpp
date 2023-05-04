#include "score_manager.h"
#include "game.h"
#include "hit_object.h"

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
      ranking('X'),
      accuracy(100.00)
{}

ScoreManager::~ScoreManager()
{}

void ScoreManager::update(HIT_TYPE hit_type, int hit_combo)
{
    switch (hit_type)
    {
    case HIT300:
        nums_hit300++;
        if (hit_combo == 1) nums_hit300g++;
        combo++;
        score += 300*combo;
        break;

    case HIT100:
        nums_hit100++;
        if (hit_combo == 1) nums_hit100k++;
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

    float ranking_param_nums_hit300 = 100*float(nums_hit300)/float(nums_hit300 + nums_hit100 + nums_hit50 + nums_hit0);
    float ranking_param_nums_hit50  = 100*float(nums_hit50) /float(nums_hit300 + nums_hit100 + nums_hit50 + nums_hit0);

    if (accuracy == 100)
        ranking = 'X';
    else if (ranking_param_nums_hit300 > 90 && ranking_param_nums_hit50 < 1 && nums_hit0 == 0)
        ranking = 'S';
    else if ((ranking_param_nums_hit300 > 80 && nums_hit0 == 0) || ranking_param_nums_hit300 > 90)
        ranking = 'A';
    else if ((ranking_param_nums_hit300 > 70 && nums_hit0 == 0) || ranking_param_nums_hit300 > 80)
        ranking = 'B';
    else if (ranking_param_nums_hit300 > 60)
        ranking = 'C';
    else
        ranking = 'D';
}

