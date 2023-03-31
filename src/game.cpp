#include "game.h"
#include "texture_manager.h"
#include "sound_manager.h"
#include "beatmap_manager.h"

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

    SoundManager::playMusic(music, musicVolume*masterVolume/100);

    init_time = SDL_GetTicks();
    std::cout << init_time << std::endl;

    while (running)
    {
        time_elapsed = SDL_GetTicks() - init_time;
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

    BeatmapManager::loadCirclesFromBeatmap("songs/1263264 katagiri - ch3rry (Short Ver)/katagiri - ch3rry (Short Ver.) (Inverse) [Blossom].osu", circles);
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
    music = SoundManager::loadAudio("songs/1263264 katagiri - ch3rry (Short Ver)/audio.mp3");

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
            if (circles.size() > 0)
                circles.back().handleClick(time_elapsed);
            break;

        case SDL_KEYDOWN:
            int input = event.key.keysym.sym;
            int K1 = SDLK_z,
                K2 = SDLK_x;
            if (input == K1 || input == K2)
            {
                cursor.handleClick();
                if (circles.size() > 0)
                    circles.back().handleClick(time_elapsed);
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
        circles.back().update(time_elapsed);
        if (circles.back().isHit())
        {
            SoundManager::playHitEffect(hitnormal, effectVolume*masterVolume/100);
            //play hit effect
            circles.pop_back();
        }
        else if (circles.back().isMiss())
        {
            //play miss effect
            circles.pop_back();
        }
    }

    cursor.update();
}

void Game::render()
{
    //clear buffer
    SDL_RenderClear(gRenderer);

    //render circle
    for (auto circle : circles)
    {
        circle.render(gRenderer, time_elapsed);
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

