#pragma once

#include <string>
#include <deque>

class HitObject;
class Game;

class BeatmapManager
{
public:
    static void loadHitObjectsFromBeatmap(std::string path, std::deque<HitObject*>& hitobjects, Game& game);
};
