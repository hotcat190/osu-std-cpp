#pragma once

#include <SDL_image.h>
#include <string>

class Game;

class TextureMananger
{
public:
    TextureMananger(Game&);
    ~TextureMananger();

    Game& game;

    SDL_Texture* loadTexture(std::string path, SDL_Renderer* ren);

    void loadTextures();
    void freeTextures();

    SDL_Texture*    cursorTexture;

    SDL_Texture*    hitcircle;
    SDL_Texture*    hitcircleoverlay;
    SDL_Texture*    defaults[10];
    SDL_Texture*    approachcircle_texture;

    SDL_Texture*    hit300;
    SDL_Texture*    hit100;
    SDL_Texture*    hit50;
    SDL_Texture*    hit0;

    SDL_Texture*    scorebar_bg;
    SDL_Texture*    scorebar_colour;

    SDL_Texture*    map_bg;

    SDL_Texture*    fail_background;
    SDL_Texture*    pause_retry;
    SDL_Texture*    pause_back;

    SDL_Texture*    spinner_circle;

};
