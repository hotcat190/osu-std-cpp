#include "sound_manager.h"

Mix_Chunk* SoundManager::loadSFX(std::string path)
{
    return Mix_LoadWAV(path.c_str());
}

Mix_Music* SoundManager::loadAudio(std::string path)
{
    return Mix_LoadMUS(path.c_str());
}

void SoundManager::playSoundEffect(Mix_Chunk* effect, int volume)
{
    Mix_Volume(-1, volume);
    Mix_PlayChannel(-1, effect, 0);
}

void SoundManager::playMusic(Mix_Music* music, int volume)
{
    Mix_VolumeMusic(volume);
    Mix_PlayMusic(music, 0);
}
