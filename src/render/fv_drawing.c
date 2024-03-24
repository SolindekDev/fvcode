/*
 * FVCode - Fast and easy code editor
 * Copyright (C) 2023-2024 FVCode Developers
 *   
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *   
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *   
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <fv/fv.h>

#include <fv/fv_vector.h>
#include <fv/fv_render.h>
#include <fv/fv_color.h>
#include <fv/fv_app.h>

#define FV_BLEND_MODE                                                                                                            \
            SDL_SetRenderDrawBlendMode(app->render->sdl_renderer, (color.a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND)   \

#define FV_SET_COLOR                                                                                 \
            SDL_SetRenderDrawColor(app->render->sdl_renderer, color.r, color.g, color.b, color.a);   \

void
FV_DrawFillRect(fv_app_t* app, fv_vector_t pos, fv_vector_t size, fv_color_t color)
{
    FV_BLEND_MODE;
    FV_SET_COLOR ;

    SDL_Rect rect = { 
        .x = pos.x , .y = pos.y, 
        .w = size.x, .h = size.y
    };
    SDL_RenderFillRect(app->render->sdl_renderer, &rect);
    return;
}

void
FV_DrawRect(fv_app_t* app, fv_vector_t pos, fv_vector_t size, fv_color_t color)
{
    FV_BLEND_MODE;
    FV_SET_COLOR ;

    SDL_Rect rect = { 
        .x = pos.x , .y = pos.y, 
        .w = size.x, .h = size.y
    };
    SDL_RenderDrawRect(app->render->sdl_renderer, &rect);
    return;
}

void
FV_DrawHorizontalLine(fv_app_t* app, fv_vector_t pos, i64 width, fv_color_t color)
{
    FV_BLEND_MODE;
    FV_SET_COLOR ;

    for (i32 i = 0; i < width; i++)
        SDL_RenderDrawPoint(app->render->sdl_renderer, pos.x + i, pos.y);
    return;
}

void
FV_DrawFillCircle(fv_app_t* app, fv_vector_t pos, u32 radius, fv_color_t color)
{
    FV_BLEND_MODE;
    FV_SET_COLOR ;

    /* Jesko's method
     * https://en.wikipedia.org/wiki/Midpoint_circle_algorithm */
    i32 x          = radius;
    i32 y          = 0;
    i32 radius_err = 1 - x;

    while (x >= y) {
        FV_DrawHorizontalLine(app, FV_NewVector(pos.x - x, pos.y - y), 2 * x, color);
        FV_DrawHorizontalLine(app, FV_NewVector(pos.x - x, pos.y + y), 2 * x, color);
        FV_DrawHorizontalLine(app, FV_NewVector(pos.x - y, pos.y - x), 2 * y, color);
        FV_DrawHorizontalLine(app, FV_NewVector(pos.x - y, pos.y + x), 2 * y, color);
        y++;

        if (radius_err < 0)
            radius_err += 2 * y + 1;
        else 
        {
            x--;
            radius_err += 2 * (y - x + 1);
        }
    }

    return;
}

void
FV_DrawCircle(fv_app_t* app, fv_vector_t pos, u32 radius, fv_color_t color)
{
    FV_BLEND_MODE;
    FV_SET_COLOR ;

    /* Jesko's method
     * https://en.wikipedia.org/wiki/Midpoint_circle_algorithm */
    i32 x          = radius;
    i32 y          = 0;
    i32 radius_err = 1 - x;

    while (x >= y) {
        SDL_RenderDrawPoint(app->render->sdl_renderer, pos.x + x, pos.y - y);
        SDL_RenderDrawPoint(app->render->sdl_renderer, pos.x - x, pos.y - y);
        SDL_RenderDrawPoint(app->render->sdl_renderer, pos.x + x, pos.y + y);
        SDL_RenderDrawPoint(app->render->sdl_renderer, pos.x - x, pos.y + y);
        SDL_RenderDrawPoint(app->render->sdl_renderer, pos.x + y, pos.y - x);
        SDL_RenderDrawPoint(app->render->sdl_renderer, pos.x - y, pos.y - x);
        SDL_RenderDrawPoint(app->render->sdl_renderer, pos.x + y, pos.y + x);
        SDL_RenderDrawPoint(app->render->sdl_renderer, pos.x - y, pos.y + x);
        y++;

        if (radius_err < 0)
            radius_err += 2 * y + 1;
        else 
        {
            x--;
            radius_err += 2 * (y - x + 1);
        }
    }

    return;
}

void
FV_DrawLine(fv_app_t* app, fv_vector_t start, fv_vector_t end, fv_color_t color)
{
    FV_BLEND_MODE;
    FV_SET_COLOR ;

    SDL_RenderDrawLine(app->render->sdl_renderer, start.x, start.y, end.x, end.y);
    return;
}

