#pragma once

#include <string>
#include <list>

#include "circle.h"

class Circle;

class BeatmapManager
{
public:
    static void loadCirclesFromBeatmap(std::string path, std::list<Circle>& circles);
};
