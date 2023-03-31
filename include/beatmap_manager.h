#pragma once

#include <string>
#include <deque>
#include <vector>

#include "circle.h"

class Circle;

class BeatmapManager
{
public:
    static void loadCirclesFromBeatmap(std::string path, std::deque<Circle>& circles);
};
