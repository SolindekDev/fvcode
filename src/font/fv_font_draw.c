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
#include <fv/fv_render.h>
#include <fv/fv_vector.h>
#include <fv/fv_alloc.h>

void 
FV_FontDrawFormat(fv_font_t* font, fv_render_t* render, 
                  fv_vector_t pos, i32 size, const char* text, ...)
{
    va_list ap;
    va_start(ap, text);
    char* buffer = FV_Calloc(FV_MAX_FORMAT_BUFFER, sizeof(char));
    vsnprintf(buffer, FV_MAX_FORMAT_BUFFER, text, ap);
    FV_FontDraw(font, render, pos, size, buffer);
    va_end(ap);
    FV_Free(buffer);
}

void
FV_FontDrawSingleCharacter(fv_font_t* font, fv_render_t* render, fv_vector_t pos,
                           i32 size, const char character)
{
    /* Set font size */
    FT_Set_Pixel_Sizes(font->font_face, 0, size);  

    if ((FT_Load_Char(font->font_face, character, FT_LOAD_RENDER)) == 1)
    {
        FV_ERROR_NO_EXIT("Couldn't load character \'%c\'", character);
        return;
    }

    FT_GlyphSlot glyph = font->font_face->glyph;

}

void 
FV_FontDraw(fv_font_t* font, fv_render_t* render, 
            fv_vector_t pos, i32 size, const char* text)
{
    /* Set font size */
    FT_Set_Pixel_Sizes(font->font_face, 0, size);  
}
