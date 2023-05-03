#include "texture_manager.h"
#include "game.h"

TextureMananger::TextureMananger(Game& _game)
    : game(_game)
{}

TextureMananger::~TextureMananger()
{}

SDL_Texture* TextureMananger::loadTexture(std::string path, SDL_Renderer* ren)
{
    SDL_Surface* tempSurface = IMG_Load(path.c_str());
    SDL_Texture* returnTexture = SDL_CreateTextureFromSurface(ren, tempSurface);
    SDL_FreeSurface(tempSurface);
    return returnTexture;
}

void TextureMananger::loadTextures()
{
    cursorTexture = TextureMananger::loadTexture("skin/WhitecatEZ/cursor.png", game.gRenderer);
    SDL_ShowCursor(false);

    hitcircle = TextureMananger::loadTexture("skin/WhitecatEZ/hitcircle.png", game.gRenderer);
    hitcircleoverlay = TextureMananger::loadTexture("skin/WhitecatEZ/hitcircleoverlay.png", game.gRenderer);
    approachcircle_texture = TextureMananger::loadTexture("skin/WhitecatEZ/approachcircle.png", game.gRenderer);

    for (int i = 0; i < 10; i++)
    {
        defaults[i] = TextureMananger::loadTexture("skin/WhitecatEZ/Defaults/default-" + std::to_string(i) + "@2x.png", game.gRenderer);
        score_entry[i] = TextureMananger::loadTexture("skin/WhitecatEZ/scoreentry-" + std::to_string(i) + "@2x.png", game.gRenderer);
    }
    score_percent = TextureMananger::loadTexture("skin/WhitecatEZ/score-percent@2x.png", game.gRenderer);
    score_dot = TextureMananger::loadTexture("skin/WhitecatEZ/scoreentry-dot@2x.png", game.gRenderer);
    score_x = TextureMananger::loadTexture("skin/WhitecatEZ/scoreentry-x@2x.png", game.gRenderer);

    spinner_circle = TextureMananger::loadTexture("skin/WhitecatEZ/spinner-circle@2x.png", game.gRenderer);

    hit300 = TextureMananger::loadTexture("skin/WhitecatEZ/hit300.png", game.gRenderer);
    hit100 = TextureMananger::loadTexture("skin/WhitecatEZ/hit100-0@2x.png", game.gRenderer);
    hit50  = TextureMananger::loadTexture("skin/WhitecatEZ/hit50-0@2x.png", game.gRenderer);
    hit0   = TextureMananger::loadTexture("skin/WhitecatEZ/hit0-0@2x.png", game.gRenderer);

    scorebar_bg = TextureMananger::loadTexture("skin/WhitecatEZ/scorebar-bg@2x.png", game.gRenderer);
    scorebar_colour = TextureMananger::loadTexture("skin/WhitecatEZ/scorebar-colour@2x.png", game.gRenderer);

    fail_background = TextureMananger::loadTexture("skin/WhitecatEZ/fail-background@2x.png", game.gRenderer);
    pause_retry = TextureMananger::loadTexture("skin/WhitecatEZ/pause-retry@2x.png", game.gRenderer);
    pause_back = TextureMananger::loadTexture("skin/WhitecatEZ/pause-back@2x.png", game.gRenderer);

    ranking_panel = TextureMananger::loadTexture("skin/WhitecatEZ/ranking-panel@2x.png", game.gRenderer);

    map_bg = TextureMananger::loadTexture("songs/321437 Kozato - Tsuki -Yue-/1234.jpg", game.gRenderer);
}

SDL_Texture* TextureMananger::createText(const std::string& text)
{
    // create a surface which contains the desired text.
    SDL_Color color = { 0xff, 0xff, 0xff, 0xff };
    auto surface = TTF_RenderText_Solid(game.gFont, text.c_str(), color);
    if (surface == nullptr) {
        std::cerr << "Unable to create a surface with a text: " << TTF_GetError() << std::endl;
        return nullptr;
    }

    // create a texture from the text surface.
    auto texture = SDL_CreateTextureFromSurface(game.gRenderer, surface);
    SDL_FreeSurface(surface);
    if (texture == nullptr) {
        std::cerr << "Unable to create texture from a text surface: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    return texture;
}

void TextureMananger::freeTextures()
{
    SDL_DestroyTexture(cursorTexture);

    SDL_DestroyTexture(hitcircle);
    SDL_DestroyTexture(hitcircleoverlay);
    SDL_DestroyTexture(approachcircle_texture);

    for (int i = 0; i < 10; i++)
    {
        SDL_DestroyTexture(defaults[i]);
    }

    SDL_DestroyTexture(hit300);
    SDL_DestroyTexture(hit100);
    SDL_DestroyTexture(hit50);
    SDL_DestroyTexture(hit0);

    SDL_DestroyTexture(scorebar_bg);
    SDL_DestroyTexture(scorebar_colour);

    SDL_DestroyTexture(fail_background);
    SDL_DestroyTexture(pause_retry);
    SDL_DestroyTexture(pause_back);

    SDL_DestroyTexture(spinner_circle);
}
