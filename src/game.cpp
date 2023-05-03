#include "game.h"

#include "texture_manager.h"
#include "sound_manager.h"
#include "beatmap_manager.h"
#include "score_manager.h"

#include "cursor.h"

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

        gScore = new ScoreManager(*this);

        health = MAX_HEALTH;

        retry = false;
        failed = false;
        passed = false;

        gSound->playMusic();

        init_time = SDL_GetTicks();
        std::cout << init_time << std::endl;

        while (!failed && !passed)
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

        if (failed)
            renderFailScreen();
        if (passed)
            renderResultScreen();

        if (retry)
        {
            while (hitobjects.size() > 0)
                hitobjects.pop_back();
            while (hiteffects.size() > 0)
                hiteffects.pop_back();
            while (render_stack.size() > 0)
                render_stack.pop_back();
            delete gScore;
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

            gSound->playSoundEffectHitNormal();

            HitEffect* hiteffect = new HitEffect(*this, render_stack.back()->position, render_stack.back()->hit_type, SDL_GetTicks());
            hiteffects.push_front(hiteffect);

            gScore->update(render_stack.back()->hit_type);

            render_stack.pop_back();
        }
        else if (render_stack.back()->isMiss())
        {
            health -= 300;

            if (health < 0)
            {
                failed = true;
                Mix_HaltMusic();
                return;
            }

            if (gScore->combo > 30)
                gSound->playSoundEffectComboBreak();

            HitEffect* hiteffect = new HitEffect(*this, render_stack.back()->position, render_stack.back()->hit_type, SDL_GetTicks());
            hiteffects.push_front(hiteffect);

            gScore->update(render_stack.back()->hit_type);

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

    //render score
    std::string score_text = std::to_string(gScore->score);
    SDL_Rect score_rect = {gWidth - 25, 0, 20, 24};
    for (auto index = score_text.end()-1; index >= score_text.begin(); index--)
    {
        char digit = *index;
        SDL_RenderCopy(gRenderer, gTexture->score_entry[digit-'0'], nullptr, &score_rect);
        score_rect.x -= score_rect.w;
    }
    //render accuracy
    std::ostringstream out;
    out.precision(2);
    out << std::fixed << (gScore->accuracy);
    std::string accuracy_text = out.str();
    SDL_Rect accuracy_rect = {gWidth - 30, 27, 20, 18};
    SDL_RenderCopy(gRenderer, gTexture->score_percent, nullptr, &accuracy_rect);
    accuracy_rect.x -= 20;
    accuracy_rect.y -= 2;
    accuracy_rect.w = 20;
    accuracy_rect.h = 24;
    for (auto index = accuracy_text.end()-1; index >= accuracy_text.begin(); index--)
    {
        char digit = *index;
        if (digit == '.')
            SDL_RenderCopy(gRenderer, gTexture->score_dot, nullptr, &accuracy_rect);
        else
            SDL_RenderCopy(gRenderer, gTexture->score_entry[digit-'0'], nullptr, &accuracy_rect);
        accuracy_rect.x -= accuracy_rect.w;
    }
    //render combo
    std::string combo_text = std::to_string(gScore->combo);
    SDL_Rect combo_rect = {0, gHeight-25, 20, 24};
    for (char digit : combo_text)
    {
        SDL_RenderCopy(gRenderer, gTexture->score_entry[digit-'0'], nullptr, &combo_rect);
        combo_rect.x += combo_rect.w;
    }
    combo_rect.w = 12;
    combo_rect.h = 24;
    SDL_RenderCopy(gRenderer, gTexture->score_x, nullptr, &combo_rect);

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

void Game::renderFailScreen()
{
    SDL_Rect fail_bg_rect = {gWidth/4, 0, gWidth/2, gHeight};
    SDL_Rect pause_retry_rect = {gWidth/4 + 50, gHeight/2 - 20, 300, 80};
    SDL_Rect pause_back_rect = {gWidth/4 + 50, gHeight/2 + 80, 300, 130};

    while (running && failed)
    {
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
            else if (event.type == SDL_MOUSEBUTTONDOWN)
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

        //Rendering
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

void Game::renderResultScreen()
{
    std::string nums_hit300_text = std::to_string(gScore->nums_hit300);
    std::string nums_hit300g_text = std::to_string(gScore->nums_hit300g);
    std::string nums_hit100_text = std::to_string(gScore->nums_hit100);
    std::string nums_hit100k_text = std::to_string(gScore->nums_hit100k);
    std::string nums_hit50_text = std::to_string(gScore->nums_hit50);
    std::string nums_hit0_text = std::to_string(gScore->nums_hit0);

    std::string max_combo_text = std::to_string(gScore->max_combo);

    SDL_Rect pause_retry_rect = {gWidth - 300, gHeight - 200, 300, 80};
    SDL_Rect pause_back_rect = {gWidth - 300, gHeight - 100, 300, 130};

    while (running && passed)
    {
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
            else if (event.type == SDL_MOUSEBUTTONDOWN)
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

        //Rendering
        SDL_RenderClear(gRenderer);

        SDL_Rect ranking_panel_rect = {0, 0, 1300, 1360};
        SDL_RenderCopy(gRenderer, gTexture->ranking_panel, nullptr, &ranking_panel_rect);

        SDL_Rect nums_hit300_rect = {110, 122, 20, 24};
        SDL_Rect nums_hit300g_rect = {408, 122, 20, 24};
        SDL_Rect nums_hit100_rect = {110, 211, 20, 24};
        SDL_Rect nums_hit100k_rect = {408, 211, 20, 24};
        SDL_Rect nums_hit50_rect = {110, 301, 20, 24};
        SDL_Rect nums_hit0_rect = {408, 301, 20, 24};
        for (char digit : nums_hit300_text)
        {
            SDL_RenderCopy(gRenderer, gTexture->score_entry[digit-'0'], nullptr, &nums_hit300_rect);
            nums_hit300_rect.x += nums_hit300_rect.w;
        }
        for (char digit : nums_hit300g_text)
        {
            SDL_RenderCopy(gRenderer, gTexture->score_entry[digit-'0'], nullptr, &nums_hit300g_rect);
            nums_hit300g_rect.x += nums_hit300g_rect.w;
        }
        for (char digit : nums_hit100_text)
        {
            SDL_RenderCopy(gRenderer, gTexture->score_entry[digit-'0'], nullptr, &nums_hit100_rect);
            nums_hit100_rect.x += nums_hit100_rect.w;
        }
        for (char digit : nums_hit100k_text)
        {
            SDL_RenderCopy(gRenderer, gTexture->score_entry[digit-'0'], nullptr, &nums_hit100k_rect);
            nums_hit100k_rect.x += nums_hit100k_rect.w;
        }
        for (char digit : nums_hit50_text)
        {
            SDL_RenderCopy(gRenderer, gTexture->score_entry[digit-'0'], nullptr, &nums_hit50_rect);
            nums_hit50_rect.x += nums_hit50_rect.w;
        }
        for (char digit : nums_hit0_text)
        {
            SDL_RenderCopy(gRenderer, gTexture->score_entry[digit-'0'], nullptr, &nums_hit0_rect);
            nums_hit0_rect.x += nums_hit0_rect.w;
        }

        SDL_Rect max_combo_rect = {110, 400, 20, 24};
        for (auto index = max_combo_text.end()-1; index >= max_combo_text.begin(); index--)
        {
            char digit = *index;
            SDL_RenderCopy(gRenderer, gTexture->score_entry[digit-'0'], nullptr, &max_combo_rect);
            max_combo_rect.x -= max_combo_rect.w;
        }

        SDL_RenderCopy(gRenderer, gTexture->pause_retry, nullptr, &pause_retry_rect);
        SDL_RenderCopy(gRenderer, gTexture->pause_back, nullptr, &pause_back_rect);

        cursor.render();

        SDL_RenderPresent(gRenderer);

        if (retry)
        {
            passed = false;
            return;
        }
    }
}



