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

#include <fv/fv_component_code_area.h>

#include <fv/fv_string.h>
#include <fv/fv_io.h>

#include <fv/fv_component_lable.h>
#include <fv/fv_component_manager.h>
#include <fv/fv_component.h>
#include <fv/fv_collisions.h>
#include <fv/fv_drawing.h>

#include <fv/fv_font_manager.h>
#include <fv/fv_font_draw.h>

static i32 standard_glyph_size;

/* This specific function will calculate position in text
 * by given mouse position. This function is widealy used in
 * fvcode so it needs to be as optimised as it can. */
fv_vector_t FV_CodeAreaPositionByMouse(fv_component_t* component, fv_vector_t mouse_position)
{
    GET_CODE_AREA(component);

    fv_vector_t text_position = FV_NewVector(0, 0);
    fv_code_area_lines_values_t values = FV_ComponentCodeAreaGetValues(code_area);

    const i32 start_y = code_area->pos.y + 5;
    const i32 start_x = values.text_pos_start;

    if (start_y > mouse_position.y || start_x > mouse_position.x)
        return FV_NewVector(-1, -1);

    /* Determite the Y cordinate in text */

    const i32 whole_y = mouse_position.y - start_y;
    const i32 line_height = code_area->font_size + code_area->line_space;

    text_position.y = (whole_y / line_height) + code_area->view_line;

    /* Determite the X cordinate in text */

    fv_array_t* _splited_code = FV_StringSplitByNewline(code_area->code_value);
    char*  current_line     = FV_GetElementFromArray(_splited_code, text_position.y);
    size_t current_line_len = strlen(current_line);

    i32    x_cord = start_x;
    i32    i      = 0;

    if (mouse_position.x < (standard_glyph_size + start_x))
        i = -1;
    else
    {
        for (i = 0; i < current_line_len; i++)
        {
            char current_line_char = current_line[i];
            i32  adv = 0;
            TTF_GlyphMetrics32(code_area->default_font->font, current_line_char, NULL, NULL, NULL, NULL, &adv);
            standard_glyph_size = adv;
            x_cord += adv;
            if (x_cord < mouse_position.x && (x_cord + adv) > mouse_position.x)
                break;
            else
                continue;
        }
    }
        
    text_position.x = (i + 1);

    return text_position;
}

/* This function is being called on every time you click 
 * the code area with text*/
void 
FV_ComponentCodeAreaSetCursorByMouse(fv_component_t* component, fv_app_t* app, SDL_Event event)
{
    GET_CODE_AREA(component);

    fv_vector_t in_text_place = FV_CodeAreaPositionByMouse(component, FV_NewVector(event.button.x, event.button.y));

    if (in_text_place.x == -1 && in_text_place.y == -1)
        return;

    code_area->cursor->x = in_text_place.x;
    code_area->cursor->y = in_text_place.y;
}