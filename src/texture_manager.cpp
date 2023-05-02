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
    }
    hit300 = TextureMananger::loadTexture("skin/WhitecatEZ/hit300.png", game.gRenderer);
    hit100 = TextureMananger::loadTexture("skin/WhitecatEZ/hit100-0@2x.png", game.gRenderer);
    hit50  = TextureMananger::loadTexture("skin/WhitecatEZ/hit50-0@2x.png", game.gRenderer);
    hit0   = TextureMananger::loadTexture("skin/WhitecatEZ/hit0-0@2x.png", game.gRenderer);

    scorebar_bg = TextureMananger::loadTexture("skin/WhitecatEZ/scorebar-bg@2x.png", game.gRenderer);
    scorebar_colour = TextureMananger::loadTexture("skin/WhitecatEZ/scorebar-colour@2x.png", game.gRenderer);

    fail_background = TextureMananger::loadTexture("skin/WhitecatEZ/fail-background@2x.png", game.gRenderer);
    pause_retry = TextureMananger::loadTexture("skin/WhitecatEZ/pause-retry@2x.png", game.gRenderer);
    pause_back = TextureMananger::loadTexture("skin/WhitecatEZ/pause-back@2x.png", game.gRenderer);

    spinner_circle = TextureMananger::loadTexture("skin/WhitecatEZ/spinner-circle@2x.png", game.gRenderer);
}
