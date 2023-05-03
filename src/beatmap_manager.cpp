#include "beatmap_manager.h"
#include "difficulty_modifiers.h"

#include "game.h"

#include "circle.h"
#include "spinner.h"

#include <fstream>
#include <sstream>
#include <vector>

const int uOffset = 48; //universal offset

void BeatmapManager::loadHitObjectsFromBeatmap(std::string path, std::deque<HitObject*>& hitobjects, Game& game)
{
    std::ifstream fin(path);
    if (!fin)
        game.log(std::cout, "unable to open beatmap file", false);

    std::string line;
    do
    {
        std::getline(fin, line);
    }
    while (line != "[HitObjects]");

    int combo = 1;
    while (std::getline(fin, line))
    {
        std::istringstream iss(line);

        std::string value;
        std::vector<std::string> substrings;

        while (std::getline(iss, value, ','))
        {
            substrings.push_back(value);
        }

        if ((std::stoi(substrings[3]) & HIT_CIRCLE) ||
            (std::stoi(substrings[3]) & SLIDER))
        {
            Circle* circle = new Circle(game);
            circle->position.x = std::stoi(substrings[0]) * 640.f/640.f + 80 + CS_scaled/2;
            circle->position.y = std::stoi(substrings[1]) * 480.f/480.f + 60 + CS_scaled/2;

            circle->time_to_hit = Uint32(std::stoi(substrings[2])) + uOffset;
            circle->time_to_appear = circle->time_to_hit - AR_scaled;

            circle->approachcircle.position = circle->position;
            circle->approachcircle.time_appear = circle->time_to_appear;

            if (std::stoi(substrings[3]) & NEW_COMBO)
            {
                combo = 1;
                circle->combo = 1;
            }
            else circle->combo = combo++;

            if (combo >= 10) combo = 1;

            hitobjects.push_front(circle);
        }
        else if (std::stoi(substrings[3]) & SPINNER)
        {
            Spinner* spinner = new Spinner(game);

            spinner->time_to_hit = Uint32(std::stoi(substrings[5])) + uOffset;
            spinner->time_to_appear = Uint32(std::stoi(substrings[2])) + uOffset;

            hitobjects.push_front(spinner);
        }
    }
}

