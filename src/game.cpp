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
      gWidth(800),
      gHeight(600),
      sdl_flags(0),
      cursor(),
      hitnormal(nullptr),
      music(nullptr),
      masterVolume(25),
      musicVolume(50),
      effectVolume(50),
      time_elapsed(0),
      health(10000)
{}

Game::~Game()
{}

void Game::renderFailScreen()
{
    while (running && failed)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type != SDL_KEYDOWN)
                continue;

            int input = event.key.keysym.sym;
            if (input == SDLK_BACKQUOTE)
                retry = true;
            break;
        }

        if (retry)
            return;

        render();
    }
}

void Game::start()
{
    init();
    running = true;

    while (running)
    {
        load();

        //Play Music
        SoundManager::playMusic(music, musicVolume*masterVolume/100);

        init_time = SDL_GetTicks();
        std::cout << init_time << std::endl;

        while (!failed)
        {
            time_elapsed = SDL_GetTicks() - init_time;
            handleEvents();

            if (!running)
                break;

            update();
            render();
        }
//        renderFailScreen();
//        if (!retry)
//            running = false;
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
}

void Game::load()
{
    loadTextures();
    loadAudio();
    loadHitObjects();
}

void Game::loadTextures()
{
    cursor.cursorTexture = TextureMananger::loadTexture("skin/WhitecatEZ/cursor.png", gRenderer);

    hitcircle = TextureMananger::loadTexture("skin/WhitecatEZ/hitcircle.png", gRenderer);
    hitcircleoverlay = TextureMananger::loadTexture("skin/WhitecatEZ/hitcircleoverlay.png", gRenderer);
    approachcircle_texture = TextureMananger::loadTexture("skin/WhitecatEZ/approachcircle.png", gRenderer);
    for (int i = 0; i < 10; i++)
    {
        defaults[i] = TextureMananger::loadTexture("skin/WhitecatEZ/Defaults/default-" + std::to_string(i) + "@2x.png", gRenderer);
    }
    hit300 = TextureMananger::loadTexture("skin/WhitecatEZ/hit300.png", gRenderer);
    hit100 = TextureMananger::loadTexture("skin/WhitecatEZ/hit100-0@2x.png", gRenderer);
    hit50  = TextureMananger::loadTexture("skin/WhitecatEZ/hit50-0@2x.png", gRenderer);
    hit0   = TextureMananger::loadTexture("skin/WhitecatEZ/hit0-0@2x.png", gRenderer);

    scorebar_bg = TextureMananger::loadTexture("skin/WhitecatEZ/scorebar-bg@2x.png", gRenderer);
    scorebar_colour = TextureMananger::loadTexture("skin/WhitecatEZ/scorebar-colour@2x.png", gRenderer);
    fail_background = TextureMananger::loadTexture("skin/WhitecatEZ/fail-background.png@2x.png", gRenderer);

    spinner_circle = TextureMananger::loadTexture("skin/WhitecatEZ/spinner-circle@2x.png", gRenderer);
}

void Game::loadAudio()
{
    hitnormal = SoundManager::loadSFX("skin/WhitecatEZ/normal-hitnormal.ogg");
    music = SoundManager::loadAudio("songs/1388906 Raphlesia & BilliumMoto - My Love/audio.mp3");
}

void Game::loadHitObjects()
{
    BeatmapManager::loadHitObjectsFromBeatmap("songs/1388906 Raphlesia & BilliumMoto - My Love/Raphlesia & BilliumMoto - My Love (Mao) [Our Love].osu", hitobjects, *this);
}

void Game::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_QUIT:
            running = false;
            break;

        case SDL_MOUSEMOTION:
            cursor.handleMotion();
            if (hitobjects.size() > 0)
                hitobjects.back()->handleMotion();
            break;

        case SDL_MOUSEBUTTONDOWN:
            cursor.handleClick();
            if (hitobjects.size() > 0)
                hitobjects.back()->handleClick();
            break;

        case SDL_KEYDOWN:
            int input = event.key.keysym.sym;
            int K1 = SDLK_z,
                K2 = SDLK_x;
            if (input == K1 || input == K2)
            {
                cursor.handleClick();
                if (hitobjects.size() > 0)
                    hitobjects.back()->handleClick();
            }
            break;
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
    if (failed)
        return;

//    health -= 1;

    if (health == 0)
    {
        failed = true;
        return;
    }

    if (hitobjects.size() > 0)
    {
        hitobjects.back()->update();
        if (hitobjects.back()->getHitObjectType() == HIT_CIRCLE)
        {
            hitobjects.back() = static_cast<Circle*>(hitobjects.back());
            if (hitobjects.back()->isHit())
            {
//                health += 1000;
                SoundManager::playSoundEffect(hitnormal, effectVolume*masterVolume/100);

                HitEffect hiteffect(*this, hitobjects.back()->position, hitobjects.back()->hit_type, SDL_GetTicks());
                hiteffects.push_front(hiteffect);

                hitobjects.pop_back();
            }
            else if (hitobjects.back()->isMiss())
            {
                //SoundManager::playSoundEffect(combo_break, effectVolume*masterVolume/100);

                HitEffect hiteffect(*this, hitobjects.back()->position, hitobjects.back()->hit_type, SDL_GetTicks());
                hiteffects.push_front(hiteffect);

                hitobjects.pop_back();
            }
        }
    }

    if (hiteffects.size() > 0)
    {
        hiteffects.back().update();
        if (hiteffects.back().time_alive > 250)
        {
            hiteffects.pop_back();
        }
    }

    cursor.update();
}

void Game::render()
{
    //clear buffer
    SDL_RenderClear(gRenderer);

    if (failed)
    {
        SDL_RenderCopy(gRenderer, fail_background, nullptr, nullptr);
        SDL_RenderPresent(gRenderer);
        return;
    }


    //render hit objects
    for (auto hitobject : hitobjects)
    {
        hitobject->render();
    }

    for (auto hiteffect : hiteffects)
    {
        hiteffect.render();
    }

    //render cursor
    cursor.render(gRenderer);

    //render scorebar
    SDL_Rect scorebar_rect = {0, 0, 1350*health/10000, 80};
    SDL_RenderCopy(gRenderer, scorebar_bg, nullptr, &scorebar_rect);

    //present buffer
    SDL_RenderPresent(gRenderer);
}

void Game::clean()
{
    Mix_FreeChunk(hitnormal);
    Mix_FreeMusic(music);
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

