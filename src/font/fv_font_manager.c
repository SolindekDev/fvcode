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

#include <fv/fv_font_manager.h>
#include <fv/fv_font_draw.h>
#include <fv/fv_alloc.h>
#include <fv/fv_array.h>

fv_font_t* 
FV_CreateNewFontAsDefault(fv_font_manager_t* font_manager, char* font_path)
{
    fv_font_t* font = FV_CreateNewFont(font_manager, font_path);
    font_manager->default_font = font;
    return font;
}

fv_font_t* 
FV_CreateNewFont(fv_font_manager_t* font_manager, char* font_path)
{
    fv_font_t* font = FV_OpenNewFont(font_manager, font_path);
    FV_TRY(FV_AppendElementToArray(font_manager->fonts, font_path));
    return font;
}

void
FV_FindAndSetFontSize(fv_font_manager_t* font_manager, char* font_path, i32 size)
{
    fv_font_t* font = FV_GetFontByPath(font_manager, font_path);
    FV_NO_NULL(font);
    FV_SetFontSize(font_manager, font, size);
    return;
}

void
FV_SetFontSize(fv_font_manager_t* font_manager, fv_font_t* font, i32 size)
{
    FV_NO_NULL(font);
    if (font->font_size != size)
    {
        TTF_SetFontSize(font->font, size);
        font->font_size = size;
    }
    return;
}

fv_font_t* 
FV_OpenNewFont(fv_font_manager_t* font_manager, char* font_path)
{
    fv_font_t* font = FV_Calloc(1, sizeof(fv_font_t));
    FV_SUCCESS("Opening a new font at \'%s\'", 
        font_path, font->font_name);
    font->font = TTF_OpenFont(font_path, FV_DEFAULT_FONT_SIZE);
    if (font->font == NULL)
        FV_ERROR("Couldn't open a new font at \'%s\'", font_path);
    font->font_name = (char*)TTF_FontFaceFamilyName(font->font);
    font->font_path = font_path;
    font->font_size = FV_DEFAULT_FONT_SIZE;
    FV_SUCCESS("New font at \'%s\' with family \'%s\'", 
        font_path, font->font_name);
    return font;
}

fv_font_t* 
FV_GetFontByPath(fv_font_manager_t* font_manager, char* font_path)
{
    FV_ARRAY_FOR(font_manager->fonts)
    {
        fv_font_t* font = FV_GetElementFromArray(font_manager->fonts, i);
        if (FV_STRCMP(font->font_path, font_path))  
            return font;
    }
    return NULL;
}

fv_font_t* 
FV_GetDefaultFont(fv_font_manager_t* font_manager)
{
    return font_manager->default_font;
}

fv_font_manager_t* 
FV_FontManagerInit()
{
    fv_font_manager_t* font_manager = FV_Calloc(1, sizeof(fv_font_manager_t));
    FV_NO_NULL(font_manager);

    if ((TTF_Init()) != 0)
        FV_ERROR("Couldn't initialize SDL_TTF2 library.", 0);
    else
        FV_SUCCESS("Successfully initalized SDL_TTF2 library.", 0);

    font_manager->fonts = FV_CreateArray(sizeof(fv_font_t*));
    return font_manager;
}