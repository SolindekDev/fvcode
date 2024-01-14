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

#ifndef __FV_COMPONENT_MANAGER_H
#define __FV_COMPONENT_MANAGER_H

#include <fv/fv.h>

#include <fv/fv_component.h>
#include <fv/fv_vector.h>
#include <fv/fv_array.h>
#include <fv/fv_app.h>

typedef struct __fv_component_manager_t {
    fv_array_t* components; /* fv_component_t* */
    fv_app_t*   parent_app;
    i32         last_id;
} fv_component_manager_t;

fv_component_manager_t* FV_CreateComponentManager(fv_app_t* app);

/* Use FV_TRY */
int FV_AppendComponent(fv_component_manager_t* manager, fv_component_t* component);

/* Use FV_TRY */
int FV_DeleteComponentByName(fv_component_manager_t* manager, const char* component_name);
/* Use FV_TRY */
int FV_DeleteComponentByID  (fv_component_manager_t* manager, i32 component_id);

void FV_EventComponents(fv_component_manager_t* manager, SDL_Event event);
void FV_RenderComponents(fv_component_manager_t* manager);
void FV_RunComponents(fv_component_manager_t* manager);

#endif /* __FV_COMPONENT_MANAGER_H */