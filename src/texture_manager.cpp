#include "texture_manager.h"

SDL_Texture* TextureMananger::loadTexture(std::string path, SDL_Renderer* ren)
{
    SDL_Surface* tempSurface = IMG_Load(path.c_str());
    SDL_Texture* returnTexture = SDL_CreateTextureFromSurface(ren, tempSurface);
    SDL_FreeSurface(tempSurface);
    return returnTexture;
}

