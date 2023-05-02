#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <string>
#include <iostream>
#include <deque>

#include "cursor.h"

class HitObject;
class HitEffect;
class TextureMananger;

class Game
{
public:
    Game();
    Game(const Game&) = delete;
    Game(Game&&) = delete;

    Game& operator=(const Game&) = delete;
    Game& operator=(Game&&) = delete;

    ~Game();

    void start();

    TextureMananger* gTexture;

    Mix_Chunk* hitnormal;
    Mix_Music* music;
    int effectVolume;
    int musicVolume;
    int masterVolume;

    int gWidth, gHeight;

    Uint32 time_elapsed;
    Uint32 delta_time;

    std::deque<HitObject*> hitobjects;
    std::deque<HitEffect*> hiteffects;

    Cursor cursor;

    SDL_Window*   gWindow;
    SDL_Renderer* gRenderer;
    _TTF_Font*    gFont;



private:
    Uint8 sdl_flags;

    Uint32 init_time;

    float health;

    bool running;
    bool failed;
    bool retry;

    void init();


    void loadAudio();
    void loadHitObjects();

    void handleEvents();
    void update();
    void render();
    void clean();
    void log(std::ostream& os, const std::string &msg, bool fatal) const;

    void renderFailScreen();
    const float MAX_HEALTH = 3000;
};
