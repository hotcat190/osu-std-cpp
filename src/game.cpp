#include "game.h"
#include "texture_manager.h"
#include "sound_manager.h"
#include "beatmap_manager.h"

int K1 = SDLK_z,
    K2 = SDLK_x;

Game::Game()
    : gWindow(nullptr),
      gRenderer(nullptr),
      gFont(nullptr),
      gWidth(640),
      gHeight(480),
      sdl_flags(0),
      cursor(),
      hitnormal(nullptr),
      music(nullptr),
      masterVolume(10),
      musicVolume(50),
      effectVolume(50),
      time_elapsed(0)
{}

Game::~Game()
{}

void Game::start()
{
    init();
    running = true;

    init_time = SDL_GetTicks();
    std::cout << init_time << std::endl;
    while (running)
    {
        handleEvents();
        update();
        render();
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

    cursor.cursorTexture = TextureMananger::loadTexture("skin/cursor.png", gRenderer);

//    Circle circle;
//    circle.hitcircle = TextureMananger::loadTexture("skin/hitcircle.png", gRenderer);
//    for (int i = 0; i < 10; i++)
//    {
//        circle.defaults[i] = TextureMananger::loadTexture("skin/Defaults/default-" + std::to_string(i) + "@2x.png", gRenderer);
//    }
//    circles.push_back(circle);

    BeatmapManager::loadCirclesFromBeatmap("songs/tutorial/nekodex - new beginnings (pishifat) [tutorial].osu", circles);
    for (Circle& circle : circles)
    {
        circle.hitcircle = TextureMananger::loadTexture("skin/hitcircle.png", gRenderer);
        circle.hitcircleoverlay = TextureMananger::loadTexture("skin/hitcircleoverlay.png", gRenderer);
        circle.approachcircle.texture = TextureMananger::loadTexture("skin/approachcircle.png", gRenderer);
        for (int i = 0; i < 10; i++)
        {
            circle.defaults[i] = TextureMananger::loadTexture("skin/Defaults/default-" + std::to_string(i) + "@2x.png", gRenderer);
        }
    }

    hitnormal = SoundManager::loadSFX("skin/normal-hitnormal.ogg");
    music = SoundManager::loadAudio("songs/tutorial/audio.mp3");

    SoundManager::playMusic(music, musicVolume*masterVolume/100);
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
            break;

        case SDL_MOUSEBUTTONDOWN:
            cursor.handleClick();
            time_elapsed = SDL_GetTicks() - init_time;
            if (time_elapsed >= circles.front().time_to_appear) circles.front().handleClick();
            break;

        case SDL_KEYDOWN:
            int input = event.key.keysym.sym;
            if (input == K1 || input == K2)
            {
                cursor.handleClick();
                if (circles.size() > 0) circles.front().handleClick();
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
    if (circles.size() > 0)
    {
        circles.front().update();
        if (circles.front().isHit())
        {
            SoundManager::playHitEffect(hitnormal, effectVolume*masterVolume/100);
            //play hit effect
            circles.pop_front();
        }
        else if (circles.front().isMiss())
        {
            //play miss effect
            circles.pop_front();
        }
    }
    cursor.update();
}

void Game::render()
{
    //clear buffer
    SDL_RenderClear(gRenderer);

    //render circle
    time_elapsed = SDL_GetTicks() - init_time;
    for (Circle circle : circles)
    {
        if (time_elapsed >= circle.time_to_appear) circle.render(gRenderer);
        else break;
    }
    //render cursor
    cursor.render(gRenderer);

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

