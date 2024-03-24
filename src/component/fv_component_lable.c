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

#include <fv/fv_component_lable.h>
#include <fv/fv_component_manager.h>
#include <fv/fv_component.h>
#include <fv/fv_collisions.h>
#include <fv/fv_drawing.h>


#include <fv/fv_font_manager.h>
#include <fv/fv_font_draw.h>

fv_component_t* 
FV_CreateComponentLable(const char* lable_text, fv_vector_t pos, 
                        fv_font_t* font, i32 font_size, fv_color_t color)
{
    fv_component_t* lable_component   = FV_CreateComponent("lable", COMPONENT_LABLE);
    lable_component->component_render = FV_ComponentLableRenderFunction;
    lable_component->component_event  = FV_ComponentLableEventFunction;
    lable_component->component_run    = FV_ComponentLableRunFunction;

    fv_component_lable_t* lable = calloc(1, sizeof(fv_component_lable_t));
    lable->lable_text           = lable_text;
    lable->font_size            = font_size;
    lable->color                = color;
    lable->font                 = font;
    lable->pos                  = pos;

    lable_component->component_additional_data = lable;
    return lable_component;
}

i32 
FV_ComponentLableRenderFunction(fv_component_t* component, fv_app_t* app)
{
    fv_component_lable_t* lable = component->component_additional_data;
    FV_RenderFont(app, lable->font, lable->font_size, 1280, lable->color, lable->pos, lable->lable_text);
    return 0;
}

i32 
FV_ComponentLableEventFunction(fv_component_t* component, fv_app_t* app, SDL_Event event)
{
    return 0;
}

i32 
FV_ComponentLableRunFunction(fv_component_t* component, fv_app_t* app)
{
    fv_component_lable_t* lable = component->component_additional_data;
    return 0;
}
