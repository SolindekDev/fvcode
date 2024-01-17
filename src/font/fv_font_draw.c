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
#include <config/config.h>

#include <fv/fv_font_draw.h>
#include <fv/fv_font_manager.h>

#include <fv/fv_app.h>
#include <fv/fv_vector.h>

#include <fv/fv_color.h>

static char g_font_draw_buffer[FV_MAX_FORMAT_BUFFER] = { 0 };

fv_vector_t FV_RenderFont(fv_app_t* app, fv_font_t* font, i32 size, i32 newline, fv_color_t color, 
                          fv_vector_t pos, const char* msg)
{
    FV_SetFontSize(app->font_manager, FV_GetDefaultFont(app->font_manager), size);
    SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font->font, msg, (SDL_Color){ color.r, color.g, color.b, color.a }, newline);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(app->render->sdl_renderer, surface);

    fv_vector_t font_size = FV_NewVector(0, 0);
    i32 font_size_x, font_size_y;
    SDL_QueryTexture(texture, NULL, NULL, 
                     &font_size_x, &font_size_y);

    font_size.x = font_size_x;
    font_size.y = font_size_y;

    SDL_Rect rect = { 
        .x = pos.x,       .y = pos.y, 
        .w = font_size.x, .h = font_size.y
    };
    SDL_RenderCopy(app->render->sdl_renderer, texture, NULL, &rect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    return font_size;
}

fv_vector_t FV_RenderFontFormat(fv_app_t* app, fv_font_t* font, i32 size, i32 newline, fv_color_t color, 
                                fv_vector_t pos, const char* fmt, ...)
{
    va_list list;
    va_start(list, fmt);
    vsprintf(g_font_draw_buffer, fmt, list);
    fv_vector_t font_size = FV_RenderFont(app, font, size, newline, color, pos, g_font_draw_buffer);
    memset(g_font_draw_buffer, 0, FV_MAX_FORMAT_BUFFER);
    va_end(list);
    return font_size;
}