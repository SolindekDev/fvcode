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

i32 
FV_ComponentCodeAreaEventFunction(fv_component_t* component, fv_app_t* app, SDL_Event event)
{
    GET_CODE_AREA(component);
    
    fv_code_area_lines_values_t values = FV_ComponentCodeAreaGetValues(code_area);

    bool mouse_collision = event.type == SDL_MOUSEBUTTONDOWN 
                            ? FV_CollisionBoxVector(FV_NewVector(values.text_pos_start, code_area->pos.y + 5), code_area->size, 
                                                    FV_NewVector(event.button.x, event.button.y), 
                                                    FV_NewVector(1, 1))
                            : false;

    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
    {
        code_area->size.x = event.window.data1;
        code_area->size.y = event.window.data2;
    }

    if (event.type == SDL_MOUSEBUTTONDOWN && mouse_collision && !code_area->focus)
    {
        FV_SUCCESS("Focus on code_area [id=%d]", component->component_id);
        code_area->focus = true;
        FV_ComponentCodeAreaSetCursorByMouse(component, app, event);
        SDL_StartTextInput();
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN && mouse_collision)
    {
        FV_ComponentCodeAreaSetCursorByMouse(component, app, event);
        code_area->mouse_button_state = true;
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN && !mouse_collision)
    {
        code_area->focus = false;
        SDL_StopTextInput();
    }

    if (event.type == SDL_MOUSEMOTION)
    {
        bool motion_collision = FV_CollisionBoxVector(FV_NewVector(values.text_pos_start, code_area->pos.y + 5), code_area->size, 
                                                      FV_NewVector(event.motion.x, event.motion.y), 
                                                      FV_NewVector(1, 1));

        if (event.type == SDL_MOUSEMOTION && motion_collision)
            FV_ComponentCodeAreaButtonMotion(component, app, event);
        else if (event.type == SDL_MOUSEMOTION && !motion_collision)
            SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
    }

    if (event.type == SDL_KEYDOWN && code_area->focus)
        FV_ComponentCodeAreaKeyDownEvent(component, app, event);

    if (event.type == SDL_MOUSEWHEEL)
        FV_ComponentCodeAreaHandleMouseWheel(component, event);

    if (event.type == SDL_TEXTINPUT)
        FV_ComponentCodeAreaTextInput(component, app, event);

    if (event.type == SDL_MOUSEBUTTONUP)
        code_area->mouse_button_state = false;

    return 0;
}
