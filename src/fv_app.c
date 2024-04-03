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

#include <fv/fv_component_manager.h>
#include <fv/fv_component_code_area.h>
#include <fv/fv_component_lable.h>
#include <fv/fv_component.h>

#include <fv/fv_font_manager.h>
#include <fv/fv_font_draw.h>

#include <fv/fv_drawing.h>
#include <fv/fv_render.h>
#include <fv/fv_color.h>
#include <fv/fv_io.h>

#include <fv/fv_main.h>
#include <fv/fv_app.h>

/* This function will initalize app structure 
 * that is used in main function for managing
 * FVCode process */
fv_app_t* 
FV_CreateApp(i32 argc, char** argv)
{
    /* Allocate app strucutre using FV alloc function */
    fv_app_t* fv = calloc(1, sizeof(fv_app_t));

    fv->argc     = argc;
    fv->argv     = argv;
    
    fv->Init     = (fv_app_init_func)&FV_AppInitFunctionDefault;
    fv->Run      = (fv_app_run_func) &FV_AppRunFunctionDefault;

    FV_SUCCESS("Created a app", 0);
    return fv;
}

/* This function does destroy app and unallocate
 * every memory */
void
FV_DestroyApp(fv_app_t* app)
{
    /* Unallocate everything by using FV_UnallocAll */
    SDL_Quit();
}

/* Wrapper for function FV_DestroyApp that will 
 * exit the program as well */
__FV_NO_RETURN__ void
FV_DestroyAppAndExit(fv_app_t* app, i32 code)
{
    FV_DestroyApp(app);
    exit(code);
}

/* Function that will initalize the app process.
 * Returns 0 on success, 1 on error */
i32 
FV_AppInitFunctionDefault(fv_app_t* app)
{
    FV_SUCCESS("Executing \'FV_AppInitFunctionDefault\'. App is being initalized.", 0);

    app->render = FV_RenderInit(app);
    FV_RenderCreateDefaultWindow(app->render);
    app->background   = FV_NewColorRGB(FV_BACKGROUND_COLOR);

    app->font_manager = FV_FontManagerInit();
    FV_SetFontSize(app->font_manager, FV_CreateNewFontAsDefault(app->font_manager, FV_DEFAULT_FONT_PATH), 24);
    
    app->component_manager = FV_CreateComponentManager(app);
    // FV_TRY(
    //     FV_AppendComponent(app->component_manager, 
    //         FV_CreateComponentCodeArea(FV_NewVector(0, 0), FV_NewVector(1280, 720), FV_NewColorRGB(19, 19, 19, 255), FV_NewColorRGB(211, 215, 207, 255), 
    //         DEFAULT_CODE, FV_GetDefaultFont(app->font_manager), 20, FV_NewColorRGB(26, 26, 26, 255), 7.0f, "<none>", "<none>"
    //     )
    // ));

    const char* def_code = FV_ReadWholeFileContent("./word_counter.ba");
    printf("%p\n", def_code);

    FV_TRY(
        FV_AppendComponent(app->component_manager,
            FV_CreateComponentCodeArea(FV_NewVector(0, 0), FV_NewVector(1280, 720), 20, 
                                       FV_GetDefaultFont(app->font_manager), (char*)def_code, "<none>"))
    );
    return 0;
}   

/* Function that will rn the app process.
 * Returns 0 on success, 1 on error */
i32 
FV_AppRunFunctionDefault (fv_app_t* app)
{
    FV_SUCCESS("Executing \'FV_AppRunFunctionDefault\'. App is running.", 0);
    FV_SUCCESS("Calling all components 'run' function", 0);
    FV_RunComponents(app->component_manager);

    u32 last_fps_update_time, start_time, end_time, frames = 0;
    f32 fps = 0;

    while (!app->render->exit)
    {
        start_time = SDL_GetTicks();

        FV_RenderClearWindow(app->render);
        FV_RenderCatchEvents(app->render);
        FV_RenderComponents(app->component_manager);
        FV_RenderSwapBuffer(app->render);

        end_time = SDL_GetTicks();
        frames++;
        fps = 1.0f / ((end_time - start_time) / 1000.0f);

        if (end_time - last_fps_update_time >= 1000)
        {
            FV_SUCCESS("FPS: %.4f", fps);
            last_fps_update_time = end_time;
            frames = 0;
        }
    }

    FV_SUCCESS("Destroying the app", 0);
    FV_DestroyApp(app);
    return 0;
}