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

#include <fv/fv_component_lable.h>
#include <fv/fv_component_manager.h>
#include <fv/fv_component.h>
#include <fv/fv_collisions.h>
#include <fv/fv_drawing.h>

#include <fv/fv_font_manager.h>
#include <fv/fv_font_draw.h>

void
FV_ComponentCodeAreaHighlight(fv_component_t* component, fv_app_t* app, SDL_Event event)
{
    GET_CODE_AREA(component);

    fv_vector_t mouse_pos = { .x = event.motion.x, 
                              .y = event.motion.y };

    fv_vector_t highlight_cursor = FV_CodeAreaPositionByMouse(component, mouse_pos);

    if (code_area->highlight->highlight_start == 0)
    {
        code_area->highlight->highlight_start = FV_ComponentCodeAreaGetAbsolutePositionOfPosition(component, highlight_cursor) + 1;
        return;
    }

    code_area->highlight->highlight_end = FV_ComponentCodeAreaGetAbsolutePositionOfPosition(component, highlight_cursor);
    if (code_area->highlight->highlight_end == strlen(code_area->code_value) &&
        highlight_cursor.y                  != code_area->splited_code->length)
    {
        code_area->highlight->highlight_end 
            = FV_ComponentCodeAreaGetAbsolutePositionOfPosition(component, 
                FV_NewVector(strlen(FV_GetElementFromArray(code_area->splited_code, highlight_cursor.y)), highlight_cursor.y));
    
        // FV_ComponentCodeAreaSetCursor(component, highlight_cursor);
    }
}

void
FV_ComponentCodeAreaButtonMotion(fv_component_t* component, fv_app_t* app, SDL_Event event)
{
    GET_CODE_AREA(component);

    /* Set IBeam cursor when mouse is on the code area */
    SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM));

    if (code_area->mouse_button_state == true)
        FV_ComponentCodeAreaHighlight(component, app, event);
}

void 
FV_ComponentCodeAreaClearHighlight(fv_component_t* component)
{
    GET_CODE_AREA(component);

    code_area->highlight->highlight_start = 0; 
    code_area->highlight->highlight_end   = 0;
}