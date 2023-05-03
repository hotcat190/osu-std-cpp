#include "game.h"

#include "texture_manager.h"
#include "sound_manager.h"
#include "beatmap_manager.h"

#include "cursor.h"

#include "hit_object.h"
#include "circle.h"
#include "spinner.h"

#include "hit_effect.h"

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
      time_elapsed(0),
      health(MAX_HEALTH)
{}

Game::~Game()
{}

void Game::start()
{
    init();
    running = true;

    gTexture->loadTextures();
    gSound->loadAudio();

    while (running)
    {
        loadHitObjects();

        gSound->playMusic();

        health = MAX_HEALTH;

        retry = false;
        failed = false;

        init_time = SDL_GetTicks();
        std::cout << init_time << std::endl;

        while (!failed)
        {
            time_elapsed = SDL_GetTicks() - init_time;
            Uint32 timer_start = time_elapsed;

            handleEvents();
            if (!running || retry)
                break;

            update();
            if (failed)
                break;

            render();

            Uint32 timer_end = SDL_GetTicks() - init_time;
            delta_time = timer_end - timer_start;
        }

        renderFailScreen();

        if (retry)
        {
            while (hitobjects.size() > 0)
                hitobjects.pop_back();
            while (hiteffects.size() > 0)
                hiteffects.pop_back();
            while (render_stack.size() > 0)
                render_stack.pop_back();
        }
        else running = false;
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

    gTexture = new TextureMananger(*this);
    gSound = new SoundManager(*this);
}

void Game::loadHitObjects()
{
    BeatmapManager::loadHitObjectsFromBeatmap("songs/321437 Kozato - Tsuki -Yue-/Kozato - Tsuki -Yue- (ktgster) [O (AR 9)].osu", hitobjects, *this);
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
            case SDL_MOUSEBUTTONDOWN:
            {
                cursor.handleClick();
                if (render_stack.size() > 0)
                    render_stack.back()->handleClick();
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

    if (health < MAX_HEALTH && health > 0 && render_stack.size() > 0)
        health -= 0.1f;

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

            gSound->playSoundEffect();

            HitEffect* hiteffect = new HitEffect(*this, render_stack.back()->position, render_stack.back()->hit_type, SDL_GetTicks());
            hiteffects.push_front(hiteffect);

            render_stack.pop_back();
        }
        else if (render_stack.back()->isMiss())
        {
            //SoundManager::playSoundEffect(combo_break, effectVolume*masterVolume/100);
            health -= 300;

            if (health < 0)
            {
                failed = true;
                Mix_HaltMusic();
                return;
            }

            HitEffect* hiteffect = new HitEffect(*this, render_stack.back()->position, render_stack.back()->hit_type, SDL_GetTicks());
            hiteffects.push_front(hiteffect);

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
}

void Game::render()
{
    //clear buffer
    SDL_RenderClear(gRenderer);

    SDL_SetTextureBlendMode(gTexture->map_bg, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(gTexture->map_bg, 25);
    SDL_RenderCopy(gRenderer, gTexture->map_bg, nullptr, nullptr);

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

void Game::renderFailScreen()
{
    while (running && failed)
    {
        SDL_Rect fail_bg_rect = {gWidth/4, 0, gWidth/2, gHeight};
        SDL_Rect pause_retry_rect = {gWidth/4 + 50, gHeight/2 - 20, 300, 80};
        SDL_Rect pause_back_rect = {gWidth/4 + 50, gHeight/2 + 80, 300, 130};

        //Handle Events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_MOUSEMOTION)
            {
                cursor.handleMotion();
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN || (event.key.keysym.sym == SDLK_z | SDLK_x))
            {
                SDL_Point cursor_pos = {cursor.position.x, cursor.position.y};

                if (SDL_PointInRect(&cursor_pos, &pause_retry_rect))
                {
                    retry = true;
                }
                else if (SDL_PointInRect(&cursor_pos, &pause_back_rect))
                {
                    running = false;
                }
            }
            else if (event.type == SDL_KEYDOWN)
            {
                int input = event.key.keysym.sym;
                if (input == SDLK_BACKQUOTE)
                    retry = true;
            }
        }

        cursor.update();

        SDL_RenderClear(gRenderer);


        SDL_RenderCopy(gRenderer, gTexture->fail_background, nullptr, &fail_bg_rect);
        SDL_RenderCopy(gRenderer, gTexture->pause_retry, nullptr, &pause_retry_rect);
        SDL_RenderCopy(gRenderer, gTexture->pause_back, nullptr, &pause_back_rect);

        cursor.render();

        SDL_RenderPresent(gRenderer);

        if (retry)
        {
            failed = false;
            return;
        }
    }
}
