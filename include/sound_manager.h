#pragma once

#include <SDL_mixer.h>
#include <string>

class Game;

class SoundManager
{
public:
    SoundManager(Game&);
    ~SoundManager();

    Game& game;

    void loadAudio();
    Mix_Chunk* loadSFX(std::string path);
    Mix_Music* loadMusic(std::string path);
    void playSoundEffect();
    void playMusic();
    void freeAudio();

    Mix_Chunk* hitnormal;
    Mix_Music* music;
    int effectVolume;
    int musicVolume;
    int masterVolume;
};
