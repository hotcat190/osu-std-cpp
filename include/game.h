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

    SDL_Texture*    getTexture_hitcircle() {return hitcircle;}
    SDL_Texture*    getTexture_default(int combo) {return defaults[combo];}

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

    void load();
    void loadTextures();
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
