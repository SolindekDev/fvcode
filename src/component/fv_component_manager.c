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

#include <fv/fv_component_manager.h>
#include <fv/fv_component.h>

#include <fv/fv_vector.h>
#include <fv/fv_array.h>
#include <fv/fv_app.h>

fv_component_manager_t* 
FV_CreateComponentManager(fv_app_t* app)
{
    fv_component_manager_t* manager = calloc(1, sizeof(fv_component_manager_t));
    FV_NO_NULL(manager);

    manager->components = FV_CreateArray(sizeof(fv_component_t*));
    manager->parent_app = app;
    manager->last_id    = 1;
    return manager;
}

/* Use FV_TRY */
i32 
FV_AppendComponent(fv_component_manager_t* manager, fv_component_t* component)
{
    component->component_id = manager->last_id;
    i32 _r = FV_AppendElementToArray(manager->components, component);
    manager->last_id++;
    return _r;
}

/* Use FV_TRY */
i32
FV_DeleteComponentByName(fv_component_manager_t* manager, const char* component_name)
{
    FV_ARRAY_FOR(manager->components)
    {
        fv_component_t* component = FV_GetElementFromArray(manager->components, i);
        if (FV_STRCMP(component->component_name, component_name))
            return FV_DeleteElementFromArray(manager->components, i);
    }

    return -1;
}

/* Use FV_TRY */
i32
FV_DeleteComponentByID(fv_component_manager_t* manager, i32 component_id)
{
    FV_ARRAY_FOR(manager->components)
    {
        fv_component_t* component = FV_GetElementFromArray(manager->components, i);
        if (component->component_id == component_id)
            return FV_DeleteElementFromArray(manager->components, i);
    }

    return -1;
}

void
FV_RenderComponents(fv_component_manager_t* manager)
{
    FV_ARRAY_FOR(manager->components)
    {
        fv_component_t* component = FV_GetElementFromArray(manager->components, i);
        if (component->component_render != NULL)
            component->component_render(component, manager->parent_app);
    }
}

void
FV_RunComponents(fv_component_manager_t* manager)
{
    FV_ARRAY_FOR(manager->components)
    {
        fv_component_t* component = FV_GetElementFromArray(manager->components, i);
        if (component->component_run != NULL)
            component->component_run(component, manager->parent_app);
    }
}

void
FV_EventComponents(fv_component_manager_t* manager, SDL_Event event)
{
    FV_ARRAY_FOR(manager->components)
    {
        fv_component_t* component = FV_GetElementFromArray(manager->components, i);
        if (component->component_event != NULL)
            component->component_event(component, manager->parent_app, event);
    }
}