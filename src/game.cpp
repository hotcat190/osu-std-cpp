#include "game.h"
#include "game_scene_render.h"

#include "texture_manager.h"
#include "sound_manager.h"
#include "beatmap_manager.h"
#include "score_manager.h"

#include "hit_object.h"
#include "circle.h"
#include "spinner.h"

#include "hit_effect.h"

#include <sstream>

Game::Game()
    : gWindow(nullptr),
      gRenderer(nullptr),
      gFont(nullptr),
      gTexture(nullptr),
      gSound(nullptr),
      gWidth(800),
      gHeight(600),
      sdl_flags(0),
      cursor(*this),
      ur_bar(*this),
      time_elapsed(0),
      time_paused(0),
      skip_to_time(0),
      health(MAX_HEALTH)
{}

Game::~Game()
{}

void Game::start()
{
    init();

    running = true;

    retry = false;
    failed = false;
    passed = false;
    paused = false;
    skipped = false;
    nofail = false;

    gTexture->loadTextures();
    gSound->loadAudio();

    while (running)
    {
        if (!paused || retry)
        {
            retry = false;
            paused = false;
            time_paused = 0;
            time_skipped = 0;
            time_unskipped = 0;

            loadHitObjects();

            gScore = new ScoreManager(*this);

            health = MAX_HEALTH;

            gSound->playMusic();

            if (hitobjects.back()->time_to_appear > 3000)
                skip_to_time = hitobjects.back()->time_to_appear - 3000;


            init_time = SDL_GetTicks();
            std::cout << init_time << std::endl;
        }
        else
        {
            paused = false;
            Mix_ResumeMusic();
        }

        while (!failed && !passed && !paused)
        {
            time_elapsed = SDL_GetTicks() - init_time - time_paused;
            if (skipped) time_elapsed += (skip_to_time);
            std::cout << time_elapsed << std::endl;

            Uint32 timer_start = time_elapsed;

            handleEvents();
            if (!running || retry || paused)
            {
                break;
            }

            update();
            if (failed)
            {
                break;
            }

            render();

            Uint32 timer_end = SDL_GetTicks() - init_time - time_paused;
            delta_time = timer_end - timer_start;
        }

        if (failed)
        {
            renderFailScreen();
        }
        else if (passed)
        {
            renderResultScreen();
        }
        else if (paused)
        {
            Uint32 timer_start = SDL_GetTicks();
            Mix_PauseMusic();
            renderPauseScreen();
            Uint32 timer_end = SDL_GetTicks();
            time_paused += timer_end - timer_start;
        }

        if (retry)
        {
            while (hitobjects.size() > 0)
                hitobjects.pop_back();
            while (hiteffects.size() > 0)
                hiteffects.pop_back();
            while (render_stack.size() > 0)
                render_stack.pop_back();
            while (ur_bar.hits.size() > 0)
                ur_bar.hits.pop_back();
            delete gScore;
            skipped = false;
        }
        else if (!paused) running = false;
    }
    clean();
}

void Game::init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        log(std::cout, "SDL_Init", false);
    log(std::cout, "SDL_Init", true);

    if (TTF_Init() != 0)
        log(std::cout, "TTF_Init", false);
    log(std::cout, "TTF_Init", true);

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0)
        log(std::cout, "Mix_OpenAudio", false);
    log(std::cout, "Mix_OpenAudio", true);

    gWindow = SDL_CreateWindow("osu!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gWidth, gHeight, sdl_flags);
    if (gWindow == nullptr)
        log(std::cout, "SDL_CreateWindow", false);
    log(std::cout, "SDL_CreateWindow", true);

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == nullptr)
        log(std::cout, "SDL_CreateRenderer", false);
    log(std::cout, "SDL_CreateRenderer", true);

    gFont = TTF_OpenFont("Aller_Lt.ttf", 1000);
    if (gFont == nullptr)
        log(std::cout, "TTF_OpenFont", false);
    log(std::cout, "TTF_OpenFont", true);

    gTexture = new TextureMananger(*this);
    gSound = new SoundManager(*this);
}

void Game::loadHitObjects()
{
    BeatmapManager::loadHitObjectsFromBeatmap("songs/321437 Kozato - Tsuki -Yue-/Kozato - Tsuki -Yue- (ktgster) [O].osu", hitobjects, *this);
}

void Game::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
            {
                running = false;
            }
            case SDL_MOUSEMOTION:
            {
                cursor.handleMotion();
                if (render_stack.size() > 0)
                    render_stack.back()->handleMotion();
                break;
            }

            case SDL_KEYDOWN:
//            {
//                int input = event.key.keysym.sym;
//                int K1 = SDLK_z,
//                    K2 = SDLK_x;
//                if (input != K1 && input != K2)
//                    break;
//            }
                if (event.key.keysym.sym == SDLK_BACKQUOTE)
                {
                    retry = true;
                    break;
                }
                else if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    paused = true;
                    break;
                }
            case SDL_MOUSEBUTTONDOWN:
            {
                cursor.handleClick();
                if (render_stack.size() > 0)
                    render_stack.back()->handleClick();
                if (time_elapsed <= skip_to_time && !skipped)
                {
                    SDL_Rect play_skip_rect = {(gWidth-1366)/2, gHeight-100-230/2, 1366,230};
                    SDL_Point point;
                    SDL_GetMouseState(&point.x, &point.y);
                    if (SDL_PointInRect(&point, &play_skip_rect))
                        skipped = true;
                }
                break;
            }

            case SDL_KEYUP:
//            {
//                int input = event.key.keysym.sym;
//                int K1 = SDLK_z,
//                    K2 = SDLK_x;
//                if (input != K1 && input != K2)
//                    break;
//            }
            case SDL_MOUSEBUTTONUP:
            {
                cursor.handleRelease();
                if (render_stack.size() > 0)
                    render_stack.back()->handleRelease();
                break;
            }
        }
    }
}

void Game::update()
{
//    if (circles.size() == 0)
//    {
//        running = false;
//        return;
//    }


    cursor.update();

    if (time_elapsed < skip_to_time && skipped)
    {
        Mix_RewindMusic();
        init_time = SDL_GetTicks()  - time_paused;
        Mix_SetMusicPosition(double(skip_to_time)/1000.d);
    }

    if (health < MAX_HEALTH && health > 0 && render_stack.size() > 0)
    {
        health -= 0.1f;
    }

    //add next hit object to render stack
    if (hitobjects.size() > 0)
    {
        hitobjects.back()->update();
        if (time_elapsed >= hitobjects.back()->time_to_appear)
        {
            render_stack.push_front(hitobjects.back());
            hitobjects.pop_back();
        }
    }

    //update the render stack
    if (render_stack.size() > 0)
    {
        render_stack.back()->update();
        if (render_stack.back()->isHit())
        {
            switch (render_stack.back()->hit_type)
            {
            case HIT300:
                health += 300;
                break;

            case HIT100:
                health += 100;
                break;

            case HIT50:
                health += 50;
                break;
            }
            if (health > MAX_HEALTH) health = MAX_HEALTH;

            gSound->playSoundEffectHitNormal();

            HitEffect* hiteffect = new HitEffect(*this, render_stack.back()->position, render_stack.back()->hit_type, SDL_GetTicks());
            hiteffects.push_front(hiteffect);

            gScore->update(render_stack.back()->hit_type, render_stack.back()->combo);

            ur_bar.update();

            render_stack.pop_back();
        }
        else if (render_stack.back()->isMiss())
        {
            if (health > 0) health -= 300;

            if (!nofail && health <= 0)
            {
                failed = true;
                Mix_HaltMusic();
            }

            if (gScore->combo > 30)
                gSound->playSoundEffectComboBreak();

            HitEffect* hiteffect = new HitEffect(*this, render_stack.back()->position, render_stack.back()->hit_type, SDL_GetTicks());
            hiteffects.push_front(hiteffect);

            gScore->update(render_stack.back()->hit_type, render_stack.back()->combo);

            render_stack.pop_back();
        }
    }

    if (hiteffects.size() > 0)
    {
        hiteffects.back()->update();
        if (hiteffects.back()->time_alive > 250)
        {
            hiteffects.pop_back();
        }
    }

    if (render_stack.size() == 0 && hitobjects.size() == 0)
        passed = true;
}

void Game::render()
{
    //clear buffer
    SDL_RenderClear(gRenderer);

    //render map bg
    SDL_SetTextureBlendMode(gTexture->map_bg, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(gTexture->map_bg, 25);
    SDL_RenderCopy(gRenderer, gTexture->map_bg, nullptr, nullptr);

    //render UR bar
    ur_bar.render();

    //render skip button
    if (time_elapsed <= skip_to_time && !skipped) gTexture->renderSkipButton();

    //render hit objects
    for (auto hitobject : render_stack)
    {
        hitobject->render();
    }

    for (auto hiteffect : hiteffects)
    {
        hiteffect->render();
    }

    //render scorebar
    SDL_Rect scorebar_bg_src = {0, 0, 980*health/MAX_HEALTH, 80};
    SDL_Rect scorebar_bg_dst = {0, 0, 490*health/MAX_HEALTH, 40};
    SDL_RenderCopy(gRenderer, gTexture->scorebar_bg, &scorebar_bg_src, &scorebar_bg_dst);

    //render score
    gTexture->renderScore();

    //render accuracy
    gTexture->renderAccuracy();

    //render ranking
    gTexture->renderRanking();

    //render combo
    gTexture->renderCombo();

    if (nofail) gTexture->renderNoFail();

    //render cursor
    cursor.render();

    //present buffer
    SDL_RenderPresent(gRenderer);
}

void Game::clean()
{
    gSound->freeAudio();
    gTexture->freeTextures();
    SDL_DestroyWindow(gWindow);
    SDL_DestroyRenderer(gRenderer);
    TTF_CloseFont(gFont);
    SDL_Quit();
}

void Game::log(std::ostream& os, const std::string &msg, bool succeed) const
{
    if (!succeed)
    {
        os << msg << " Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }
    os << msg << " completed." << std::endl;
}

