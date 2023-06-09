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
    SDL_Texture* createText(const std::string& text);

    void loadTextures();
    void freeTextures();

    void renderScore();
    void renderAccuracy();
    void renderCombo();
    void renderRanking();
    void renderSkipButton();
    void renderNoFail();

    SDL_Texture*    cursorTexture;

    SDL_Texture*    hitcircle;
    SDL_Texture*    hitcircleoverlay;
    SDL_Texture*    defaults[10];
    SDL_Texture*    approachcircle_texture;
    SDL_Texture*    spinner_circle;

    SDL_Texture*    hit300;
    SDL_Texture*    hit100;
    SDL_Texture*    hit50;
    SDL_Texture*    hit0;

    SDL_Texture*    score_entry[10];
    SDL_Texture*    score_percent;
    SDL_Texture*    score_dot;
    SDL_Texture*    score_x;

    SDL_Texture*    ranking_x;
    SDL_Texture*    ranking_s;
    SDL_Texture*    ranking_a;
    SDL_Texture*    ranking_b;
    SDL_Texture*    ranking_c;
    SDL_Texture*    ranking_d;

    SDL_Texture*    ranking_x_small;
    SDL_Texture*    ranking_s_small;
    SDL_Texture*    ranking_a_small;
    SDL_Texture*    ranking_b_small;
    SDL_Texture*    ranking_c_small;
    SDL_Texture*    ranking_d_small;

    SDL_Texture*    scorebar_bg;
    SDL_Texture*    scorebar_colour;

    SDL_Texture*    play_skip;

    SDL_Texture*    fail_background;
    SDL_Texture*    pause_overlay;
    SDL_Texture*    pause_continue;
    SDL_Texture*    pause_retry;
    SDL_Texture*    pause_back;

    SDL_Texture*    mod_no_fail;

    SDL_Texture*    ranking_panel;

    SDL_Texture*    map_bg;
};
