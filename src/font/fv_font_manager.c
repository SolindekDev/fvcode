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

#include <fv/fv_font_manager.h>
#include <fv/fv_font_draw.h>
#include <fv/fv_alloc.h>
#include <fv/fv_msg.h>

#include <ft2build.h>
#include FT_FREETYPE_H

fv_font_t*
FV_OpenNewFont(fv_font_manager_t* font_manager, char* font_path)
{
    fv_font_t* font = FV_Calloc(1, sizeof(fv_font_t));
    if ((FT_New_Face(font_manager->freetype, font_path, 0, &font->font_face)) != 0)
        FV_ERROR("Couldn't open font named \'%s\'", font_path);
    return font;
}

fv_font_t*
FV_CreateNewFont(fv_font_manager_t* font_manager, char* font_path)
{
    fv_font_t* font = FV_OpenNewFont(font_manager, font_path);
    FV_TRY(FV_AppendElementToArray(font_manager->fonts, font_path));
    return font;
}

fv_font_t*
FV_CreateNewFontAsDefault(fv_font_manager_t* font_manager, char* font_path)
{
    fv_font_t* font = FV_CreateNewFont(font_manager, font_path);
    font_manager->default_font = font;
    return font;
}

fv_font_manager_t* 
FV_FontManagerInit()
{
    fv_font_manager_t* font_manager = FV_Calloc(1, sizeof(fv_font_manager_t));

    if ((FT_Init_FreeType(&font_manager->freetype)) != 0)
        FV_ERROR("Couldn't initialize FreeType library.", 0);
    else
        FV_SUCCESS("Successfully initalized FreeType library.", 0);

    font_manager->fonts = FV_CreateArray(sizeof(fv_font_t*));
    return font_manager;
}