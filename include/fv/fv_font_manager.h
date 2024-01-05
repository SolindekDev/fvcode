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

#include <ft2build.h>
#include FT_FREETYPE_H

typedef struct __fv_font_t {
    FT_Face font_face;
    char*   font_path;
    char*   font_name;
} fv_font_t;

typedef struct __fv_font_manager_t {
    fv_font_t*  default_font;
    FT_Library  freetype;
    fv_array_t* fonts /* fv_font_t* */; 
} fv_font_manager_t;

fv_font_manager_t* FV_FontManagerInit();

fv_font_t* FV_CreateNewFontStructure();

fv_font_t* FV_CreateNewFontAsDefault(fv_font_manager_t* font_manager, char* font_path);
fv_font_t* FV_CreateNewFont(fv_font_manager_t* font_manager, char* font_path);
fv_font_t* FV_OpenNewFont(fv_font_manager_t* font_manager, char* font_path);

fv_font_t* FV_GetFontByName(fv_font_manager_t* font_manager, char* font_name);
fv_font_t* FV_GetFontByPath(fv_font_manager_t* font_manager, char* font_path);
fv_font_t* FV_GetDefaultFont(fv_font_manager_t* font_manager);

#endif /* __FV_FONT_MANAGER_H */