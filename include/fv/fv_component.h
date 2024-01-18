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

#ifndef __FV_COMPONENT_H
#define __FV_COMPONENT_H

#include <fv/fv.h>
#include <fv/fv_vector.h>

typedef struct __fv_component_t fv_component_t;
typedef struct __fv_app_t       fv_app_t;

typedef int (*fv_component_render_f)(fv_component_t*, fv_app_t*);
typedef int (*fv_component_event_f) (fv_component_t*, fv_app_t*, SDL_Event);
typedef int (*fv_component_run_f)   (fv_component_t*, fv_app_t*);

typedef enum __fv_component_kind_t {
    COMPONENT_LABLE,
    COMPONENT_TEXTBOX
} fv_component_kind_t;

typedef struct __fv_component_t {
    const char*         component_name;
    fv_component_kind_t component_kind;
    i32                 component_id;

    /* functions */
    fv_component_render_f component_render;
    fv_component_event_f  component_event;
    fv_component_run_f    component_run;

    /* depends on the component kind it can be cast to 
     * fv_component_lable_t and other.. */
    void* component_additional_data;
} fv_component_t;

fv_component_t* FV_CreateComponent(const char* component_name, fv_component_kind_t component_kind);

void FV_SetComponentRenderFunction(fv_component_t* component, fv_component_render_f render_func);
void FV_SetComponentEventFunction (fv_component_t* component, fv_component_event_f  event_func);
void FV_SetComponentRunFunction   (fv_component_t* component, fv_component_run_f    run_func);

#endif /* __FV_COMPONENT_H */