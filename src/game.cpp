#include "game.h"
#include "texture_manager.h"
#include "sfx_manager.h"

Game::Game()
    : gWindow(nullptr),
      gRenderer(nullptr),
      gFont(nullptr),
      gWidth(1280),
      gHeight(720),
      sdl_flags(0),
      cursor(),
      hitnormal(nullptr)
{}

Game::~Game()
{}

void Game::start()
{
    init();
    running = true;
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

    Circle circle;
    circle.hitcircle = TextureMananger::loadTexture("skin/hitcircle.png", gRenderer);
    for (int i = 0; i < 10; i++)
    {
        circle.defaults[i] = TextureMananger::loadTexture("skin/Defaults/default-" + std::to_string(i) + "@2x.png", gRenderer);
    }
    circles.push_back(circle);

    hitnormal = SFXManager::loadSFX("skin/normal-hitnormal.ogg");

}

void Game::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch(event.type)
    {
    case SDL_QUIT: case SDL_SCANCODE_ESCAPE:
        running = false;
        break;

    case SDL_MOUSEMOTION:
        cursor.handleMotion();
        break;

    case SDL_MOUSEBUTTONDOWN:
        cursor.handleClick();
        if (circles.size() > 0) circles.front().handleClick();
        break;

    default:
        break;
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
        if (circles.front().hit)
        {
            SFXManager::playHitEffect(hitnormal);
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
    for (auto circle : circles) circle.render(gRenderer);

    //render cursor
    cursor.render(gRenderer);

    //present buffer
    SDL_RenderPresent(gRenderer);
}

void Game::clean()
{
    SDL_DestroyWindow(gWindow);
    SDL_DestroyRenderer(gRenderer);
    SDL_Quit();
}

void Game::log(std::ostream& os, const std::string &msg, bool succeed)
{
    if (!succeed)
    {
        os << msg << " Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }
    os << msg << " completed." << std::endl;
}
