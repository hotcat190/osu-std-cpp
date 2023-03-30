#include "beatmap_manager.h"
#include <fstream>
#include <sstream>
#include <vector>

void BeatmapManager::loadCirclesFromBeatmap(std::string path, std::list<Circle>& circles)
{
    std::ifstream fin(path);
    std::string line;
    do
    {
        std::getline(fin, line);
    }
    while (line != "[HitObjects]");

    while (std::getline(fin, line))
    {
        std::istringstream iss(line);

        std::string value;
        std::vector<std::string> substrings;

        while (std::getline(iss, value, ','))
        {
            substrings.push_back(value);
        }

        Circle circle;
        circle.position.x = std::stoi(substrings[0]);
        circle.position.y = std::stoi(substrings[1]);

        circle.time_to_hit = Uint32(std::stoi(substrings[2]));
        circle.time_to_appear = circle.time_to_hit - 900;

        circle.approachcircle.position = circle.position;
        circle.approachcircle.time_appear = circle.time_to_appear;

        circles.push_back(circle);
    }
}

