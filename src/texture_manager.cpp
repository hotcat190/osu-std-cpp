#include "texture_manager.h"
#include "game.h"
#include "score_manager.h"

#include <sstream>

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

    hitcircle              = TextureMananger::loadTexture("skin/WhitecatEZ/hitcircle.png", game.gRenderer);
    hitcircleoverlay       = TextureMananger::loadTexture("skin/WhitecatEZ/hitcircleoverlay.png", game.gRenderer);
    approachcircle_texture = TextureMananger::loadTexture("skin/WhitecatEZ/approachcircle.png", game.gRenderer);
    spinner_circle         = TextureMananger::loadTexture("skin/WhitecatEZ/spinner-circle@2x.png", game.gRenderer);

    for (int i = 0; i < 10; i++)
    {
        defaults[i] = TextureMananger::loadTexture("skin/WhitecatEZ/Defaults/default-" + std::to_string(i) + "@2x.png", game.gRenderer);
        score_entry[i] = TextureMananger::loadTexture("skin/WhitecatEZ/scoreentry-" + std::to_string(i) + "@2x.png", game.gRenderer);
    }
    score_percent = TextureMananger::loadTexture("skin/WhitecatEZ/score-percent@2x.png", game.gRenderer);
    score_dot     = TextureMananger::loadTexture("skin/WhitecatEZ/scoreentry-dot@2x.png", game.gRenderer);
    score_x       = TextureMananger::loadTexture("skin/WhitecatEZ/scoreentry-x@2x.png", game.gRenderer);

    hit300 = TextureMananger::loadTexture("skin/WhitecatEZ/hit300.png", game.gRenderer);
    hit100 = TextureMananger::loadTexture("skin/WhitecatEZ/hit100-0@2x.png", game.gRenderer);
    hit50  = TextureMananger::loadTexture("skin/WhitecatEZ/hit50-0@2x.png", game.gRenderer);
    hit0   = TextureMananger::loadTexture("skin/WhitecatEZ/hit0-0@2x.png", game.gRenderer);

    scorebar_bg = TextureMananger::loadTexture("skin/WhitecatEZ/scorebar-bg@2x.png", game.gRenderer);
    scorebar_colour = TextureMananger::loadTexture("skin/WhitecatEZ/scorebar-colour@2x.png", game.gRenderer);

    fail_background = TextureMananger::loadTexture("skin/WhitecatEZ/fail-background@2x.png", game.gRenderer);
    pause_overlay   = TextureMananger::loadTexture("skin/WhitecatEZ/pause-overlay@2x.png", game.gRenderer);
    pause_continue  = TextureMananger::loadTexture("skin/WhitecatEZ/pause-continue@2x.png", game.gRenderer);
    pause_retry     = TextureMananger::loadTexture("skin/WhitecatEZ/pause-retry@2x.png", game.gRenderer);
    pause_back      = TextureMananger::loadTexture("skin/WhitecatEZ/pause-back@2x.png", game.gRenderer);

    ranking_panel = TextureMananger::loadTexture("skin/WhitecatEZ/ranking-panel@2x.png", game.gRenderer);

    ranking_x = TextureMananger::loadTexture("skin/WhitecatEZ/ranking-X@2x.png", game.gRenderer);
    ranking_s = TextureMananger::loadTexture("skin/WhitecatEZ/ranking-S@2x.png", game.gRenderer);
    ranking_a = TextureMananger::loadTexture("skin/WhitecatEZ/ranking-A@2x.png", game.gRenderer);
    ranking_b = TextureMananger::loadTexture("skin/WhitecatEZ/ranking-B@2x.png", game.gRenderer);
    ranking_c = TextureMananger::loadTexture("skin/WhitecatEZ/ranking-C@2x.png", game.gRenderer);
    ranking_d = TextureMananger::loadTexture("skin/WhitecatEZ/ranking-D@2x.png", game.gRenderer);

    ranking_x_small = TextureMananger::loadTexture("skin/WhitecatEZ/ranking-X-small@2x.png", game.gRenderer);
    ranking_s_small = TextureMananger::loadTexture("skin/WhitecatEZ/ranking-S-small@2x.png", game.gRenderer);
    ranking_a_small = TextureMananger::loadTexture("skin/WhitecatEZ/ranking-A-small@2x.png", game.gRenderer);
    ranking_b_small = TextureMananger::loadTexture("skin/WhitecatEZ/ranking-B-small@2x.png", game.gRenderer);
    ranking_c_small = TextureMananger::loadTexture("skin/WhitecatEZ/ranking-C-small@2x.png", game.gRenderer);
    ranking_d_small = TextureMananger::loadTexture("skin/WhitecatEZ/ranking-D-small@2x.png", game.gRenderer);

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

void TextureMananger::renderScore()
{
    std::string score_text = std::to_string(game.gScore->score);
    SDL_Rect score_rect = {game.gWidth - 25, 5, 20, 24};
    for (auto index = score_text.end()-1; index >= score_text.begin(); index--)
    {
        char digit = *index;
        SDL_RenderCopy(game.gRenderer, score_entry[digit-'0'], nullptr, &score_rect);
        score_rect.x -= score_rect.w;
    }
}

void TextureMananger::renderAccuracy()
{
    std::ostringstream out;
    out.precision(2);
    out << std::fixed << (game.gScore->accuracy);
    std::string accuracy_text = out.str();

    SDL_Rect accuracy_rect = {game.gWidth - 25, 35, 15, 15};
    SDL_RenderCopy(game.gRenderer, score_percent, nullptr, &accuracy_rect);

    accuracy_rect.x -= 20;
    accuracy_rect.y -= 5;
    accuracy_rect.w = 20;
    accuracy_rect.h = 24;

    for (auto index = accuracy_text.end()-1; index >= accuracy_text.begin(); index--)
    {
        char digit = *index;
        if (digit == '.')
            SDL_RenderCopy(game.gRenderer, score_dot, nullptr, &accuracy_rect);
        else
            SDL_RenderCopy(game.gRenderer, score_entry[digit-'0'], nullptr, &accuracy_rect);
        accuracy_rect.x -= accuracy_rect.w;
    }
}

void TextureMananger::renderCombo()
{
    std::string combo_text = std::to_string(game.gScore->combo);
    SDL_Rect combo_rect = {0, game.gHeight-25, 20, 24};
    for (char digit : combo_text)
    {
        SDL_RenderCopy(game.gRenderer, score_entry[digit-'0'], nullptr, &combo_rect);
        combo_rect.x += combo_rect.w;
    }
    combo_rect.w = 12;
    combo_rect.h = 24;
    SDL_RenderCopy(game.gRenderer, score_x, nullptr, &combo_rect);
}

void TextureMananger::renderRanking()
{
    SDL_Texture* ranking_texture = nullptr;
    switch (game.gScore->ranking)
    {
    case 'X':
        ranking_texture = ranking_x_small;
        break;
    case 'S':
        ranking_texture = ranking_s_small;
        break;
    case 'A':
        ranking_texture = ranking_a_small;
        break;
    case 'B':
        ranking_texture = ranking_b_small;
        break;
    case 'C':
        ranking_texture = ranking_c_small;
        break;
    case 'D':
        ranking_texture = ranking_d_small;
        break;
    }
    SDL_Rect ranking_rect = {game.gWidth - 25, 55, 20, 25};
    SDL_RenderCopy(game.gRenderer, ranking_texture, nullptr, &ranking_rect);
}
