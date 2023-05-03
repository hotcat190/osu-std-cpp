#include "sound_manager.h"
#include "game.h"

SoundManager::SoundManager(Game& _game)
    : game(_game),
      hitnormal(nullptr),
      combobreak(nullptr),
      music(nullptr),
      masterVolume(25),
      musicVolume(50),
      effectVolume(50)
{}

SoundManager::~SoundManager()
{}

void SoundManager::loadAudio()
{
    hitnormal = SoundManager::loadSFX("skin/WhitecatEZ/normal-hitnormal.ogg");
    combobreak = SoundManager::loadSFX("skin/WhitecatEZ/combobreak.wav");
    music = SoundManager::loadMusic("songs/321437 Kozato - Tsuki -Yue-/Kozato -  -Yue-.mp3");
}

Mix_Chunk* SoundManager::loadSFX(std::string path)
{
    return Mix_LoadWAV(path.c_str());
}

Mix_Music* SoundManager::loadMusic(std::string path)
{
    return Mix_LoadMUS(path.c_str());
}

void SoundManager::playSoundEffectHitNormal()
{
    Mix_Volume(-1, effectVolume*masterVolume/100);
    Mix_PlayChannel(-1, hitnormal, 0);
}

void SoundManager::playSoundEffectComboBreak()
{
    Mix_Volume(-1, effectVolume*masterVolume/100);
    Mix_PlayChannel(-1, combobreak, 0);
}

void SoundManager::playMusic()
{
    Mix_VolumeMusic(musicVolume*masterVolume/100);
    Mix_PlayMusic(music, 0);
}

void SoundManager::freeAudio()
{
    Mix_FreeChunk(hitnormal);
    Mix_FreeMusic(music);
}
