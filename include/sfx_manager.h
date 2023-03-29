#pragma once

#include <SDL_mixer.h>
#include <string>

class SFXManager
{
public:
    static Mix_Chunk* loadSFX(std::string path);
    static void playHitEffect(Mix_Chunk* effect);
};
