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

#ifndef __FV_COMPONENT_LABLE_H
#define __FV_COMPONENT_LABLE_H

#include <fv/fv.h>

#include <fv/fv_component_manager.h>
#include <fv/fv_component.h>

#include <fv/fv_font_manager.h>
#include <fv/fv_font_draw.h>

typedef struct __fv_component_lable_t {
    const char* lable_text;
    fv_vector_t pos;
    fv_font_t*  font;
    fv_color_t  color;
    i32         font_size;
} fv_component_lable_t;

fv_component_t* FV_CreateComponentLable(const char* lable_text, fv_vector_t pos, fv_font_t* font, i32 font_size, fv_color_t color);

i32 FV_ComponentLableRenderFunction(fv_component_t* component, fv_app_t* app);
i32 FV_ComponentLableEventFunction (fv_component_t* component, fv_app_t* app, SDL_Event event);
i32 FV_ComponentLableRunFunction   (fv_component_t* component, fv_app_t* app);

#endif /* __FV_COMPONENT_LABLE_H */