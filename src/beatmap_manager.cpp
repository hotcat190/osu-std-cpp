#include "beatmap_manager.h"
#include "difficulty_modifiers.h"

#include <fstream>
#include <sstream>

void BeatmapManager::loadCirclesFromBeatmap(std::string path, std::deque<Circle>& circles)
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
        circle.position.x = std::stoi(substrings[0]) * 640.f/640.f + 80 + CS_scaled/2;
        circle.position.y = std::stoi(substrings[1]) * 480.f/480.f + 60 + CS_scaled/2;

        int uOffset = 5; //universal offset
        circle.time_to_hit = Uint32(std::stoi(substrings[2])) + uOffset;
        circle.time_to_appear = circle.time_to_hit - AR_scaled;

        circle.approachcircle.position = circle.position;
        circle.approachcircle.time_appear = circle.time_to_appear;

        circles.push_front(circle);
    }
}

