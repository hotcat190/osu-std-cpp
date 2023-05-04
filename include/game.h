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
class SoundManager;
class ScoreManager;

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
    SoundManager* gSound;
    ScoreManager* gScore;

    int gWidth, gHeight;

    Uint32 time_elapsed;
    Uint32 delta_time;
    Uint32 time_paused;

    std::deque<HitObject*> hitobjects;
    std::deque<HitObject*> render_stack;
    std::deque<HitEffect*> hiteffects;

    Cursor cursor;

    SDL_Window*   gWindow;
    SDL_Renderer* gRenderer;
    TTF_Font*     gFont;

    void log(std::ostream& os, const std::string &msg, bool fatal) const;

private:
    Uint8 sdl_flags;

    Uint32 init_time;

    float health;
    const float MAX_HEALTH = 3000;

    bool running;
    bool failed;
    bool retry;
    bool passed;
    bool paused;

    void init();

    void loadAudio();
    void loadHitObjects();

    void handleEvents();
    void update();
    void render();

    void clean();

    void renderFailScreen();
    void renderResultScreen();
    void renderPauseScreen();
};
