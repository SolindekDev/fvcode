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

#include <fv/fv_component.h>
#include <fv/fv_alloc.h>

fv_component_t* 
FV_CreateComponent(const char* component_name, fv_component_kind_t component_kind)
{
    fv_component_t* component = FV_Calloc(1, sizeof(fv_component_t));
    FV_NO_NULL(component);

    component->component_name = component_name;
    component->component_kind = component_kind;
    return component;
}

void 
FV_SetComponentRenderFunction(fv_component_t* component, fv_component_render_f render_func)
{
    FV_NO_NULL(render_func);
    component->component_render = render_func;
}

void 
FV_SetComponentEventFunction(fv_component_t* component, fv_component_event_f event_func)
{
    FV_NO_NULL(event_func);
    component->component_event = event_func;
}

void 
FV_SetComponentRunFunction(fv_component_t* component, fv_component_run_f run_func)
{
    FV_NO_NULL(run_func);
    component->component_run = run_func;
}
