#include "sfx_manager.h"

Mix_Chunk* SFXManager::loadSFX(std::string path)
{
    return Mix_LoadWAV(path.c_str());
}

void SFXManager::playHitEffect(Mix_Chunk* effect)
{
    Mix_PlayChannel(-1, effect, 0);
}
