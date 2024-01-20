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

#ifndef __FV_APP_H
#define __FV_APP_H

#include <fv/fv.h>

#include <fv/fv_component_manager.h>
#include <fv/fv_font_manager.h>
#include <fv/fv_render.h>
#include <fv/fv_color.h>

typedef int (*fv_app_init_func)(void* app);
typedef int (*fv_app_run_func) (void* app);

typedef struct __fv_component_manager_t fv_component_manager_t;
typedef struct __fv_render_t fv_render_t;

typedef struct __fv_app_t {
    char** argv;
    i32    argc;

    fv_app_run_func  Run;
    fv_app_init_func Init;

    fv_render_t* render;
    fv_color_t   background;

    fv_component_manager_t* component_manager;
    fv_font_manager_t* font_manager;

    bool need_redraw;
} fv_app_t;

fv_app_t* FV_CreateApp(i32 argc, char** argv);

__FV_NO_RETURN__ void FV_DestroyAppAndExit(fv_app_t* app, i32 code);
                 void FV_DestroyApp(fv_app_t* app);

int FV_AppInitFunctionDefault(fv_app_t* app);
int FV_AppRunFunctionDefault(fv_app_t* app);

#endif /* __FV_APP_H */