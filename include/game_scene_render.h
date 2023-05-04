#pragma once

#include "texture_manager.h"
#include "sound_manager.h"
#include "beatmap_manager.h"
#include "score_manager.h"

#include <sstream>

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

        //update
        cursor.update();

        //Rendering
        SDL_RenderClear(gRenderer);

        SDL_SetTextureBlendMode(gTexture->map_bg, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(gTexture->map_bg, 25);
        SDL_RenderCopy(gRenderer, gTexture->map_bg, nullptr, nullptr);

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

        //ranking panel
        SDL_Rect ranking_panel_rect = {0, 0, 1300, 1360};
        SDL_RenderCopy(gRenderer, gTexture->ranking_panel, nullptr, &ranking_panel_rect);

        //hit type
        SDL_Rect nums_hit300_rect = {110, 125, 20, 24};
        SDL_Rect nums_hit300g_rect = {408, 125, 20, 24};
        SDL_Rect nums_hit100_rect = {110, 215, 20, 24};
        SDL_Rect nums_hit100k_rect = {408, 215, 20, 24};
        SDL_Rect nums_hit50_rect = {110, 305, 20, 24};
        SDL_Rect nums_hit0_rect = {408, 305, 20, 24};

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

        //score
        std::string score_text = std::to_string(gScore->score);
        SDL_Rect score_rect = {200, 32, 20, 24};
        for (char digit : score_text)
        {
            SDL_RenderCopy(gRenderer, gTexture->score_entry[digit-'0'], nullptr, &score_rect);
            score_rect.x += score_rect.w;
        }

        //combo
        SDL_Rect max_combo_rect = {110, 405, 20, 24};
        for (auto index = max_combo_text.end()-1; index >= max_combo_text.begin(); index--)
        {
            char digit = *index;
            SDL_RenderCopy(gRenderer, gTexture->score_entry[digit-'0'], nullptr, &max_combo_rect);
            max_combo_rect.x -= max_combo_rect.w;
        }

        //accuracy
        std::ostringstream out;
        out.precision(2);
        out << std::fixed << (gScore->accuracy);
        std::string accuracy_text = out.str();

        SDL_Rect accuracy_rect = {408, 405, 20, 18};
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

        //ranking
        SDL_Texture* ranking_texture = nullptr;
        switch (gScore->ranking)
        {
        case 'X':
            ranking_texture = gTexture->ranking_x;
            break;
        case 'S':
            ranking_texture = gTexture->ranking_s;
            break;
        case 'A':
            ranking_texture = gTexture->ranking_a;
            break;
        case 'B':
            ranking_texture = gTexture->ranking_b;
            break;
        case 'C':
            ranking_texture = gTexture->ranking_c;
            break;
        case 'D':
            ranking_texture = gTexture->ranking_d;
            break;
        }
        SDL_Rect ranking_rect = {620, -400, 330, 1020};
        SDL_RenderCopy(gRenderer, ranking_texture, nullptr, &ranking_rect);

        //retry & quit
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

