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
#include <fv/fv_string.h>
#include <fv/fv_drawing.h>

#include <fv/fv_font_manager.h>
#include <fv/fv_font_draw.h>

void 
FV_ComponentCodeAreaHandleMouseWheel(fv_component_t* component, SDL_Event event)
{
    GET_CODE_AREA(component);

    const int change_view_line = abs(event.wheel.y);

    if (event.wheel.y < 0)
    {
        for (i32 i = 0; i < change_view_line; i++)
            FV_ComponentCodeAreaIncrementViewLine(component);
    }
    else if (event.wheel.y > 0)
    {
        for (i32 i = 0; i < change_view_line; i++)
            FV_ComponentCodeAreaDecrementViewLine(component);
    }
}

void 
FV_ComponentCodeAreaIncrementViewLine(fv_component_t* component)
{
    GET_CODE_AREA(component);

    if (code_area->view_line != (code_area->splited_code->length - 1))
        code_area->view_line++;
}

void 
FV_ComponentCodeAreaDecrementViewLine(fv_component_t* component)
{
    GET_CODE_AREA(component);

    if (code_area->view_line != 0)
        code_area->view_line--;
}

void 
FV_ComponentCodeAreaRenderScrollBar(fv_component_t* component, fv_app_t* app)
{
    GET_CODE_AREA(component);

    const i32 max_scroll_value = code_area->splited_code->length;

    const i32 scroll_bar_height = code_area->size.y;
    const i32 track_height      = scroll_bar_height / max_scroll_value
}