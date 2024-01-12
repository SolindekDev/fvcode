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

#ifndef __FV_FONT_MANAGER_H
#define __FV_FONT_MANAGER_H

#include <fv/fv.h>
#include <fv/fv_array.h>

#include <SDL2/SDL_ttf.h>

typedef struct __fv_font_t {
    TTF_Font* font;
    char*     font_path;
    char*     font_name;
    i32       font_size;
} fv_font_t;

typedef struct __fv_font_manager_t {
    fv_font_t*  default_font;
    fv_array_t* fonts /* fv_font_t* */; 
} fv_font_manager_t;

fv_font_t* FV_CreateNewFontAsDefault(fv_font_manager_t* font_manager, char* font_path);
fv_font_t* FV_CreateNewFont(fv_font_manager_t* font_manager, char* font_path);
fv_font_t* FV_OpenNewFont(fv_font_manager_t* font_manager, char* font_path);

void FV_FindAndSetFontSize(fv_font_manager_t* font_manager, char* font_path, i32 size);
void FV_SetFontSize(fv_font_manager_t* font_manager, fv_font_t* font, i32 size);

fv_font_t* FV_GetFontByPath(fv_font_manager_t* font_manager, char* font_path);
fv_font_t* FV_GetDefaultFont(fv_font_manager_t* font_manager);

fv_font_manager_t* FV_FontManagerInit();

#endif /* __FV_FONT_MANAGER_H */