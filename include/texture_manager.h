#pragma once

#include <SDL_image.h>
#include <string>

class TextureMananger
{
public:
    static SDL_Texture* loadTexture(std::string path, SDL_Renderer* ren);

};
