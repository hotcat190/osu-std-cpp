#pragma once

#include <SDL_mixer.h>
#include <string>

class SoundManager
{
public:
    static Mix_Chunk* loadSFX(std::string path);
    static Mix_Music* loadAudio(std::string path);
    static void playHitEffect(Mix_Chunk* effect, int volume);
    static void playMusic(Mix_Music* music, int volume);
};
