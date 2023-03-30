#pragma once

class BeatmapManager
{
public:
    static void loadCirclesFromBeatmap(std::string path, std::queue<Circle>& circles);
};
