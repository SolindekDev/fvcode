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

#include <fv/fv_render.h>
#include <fv/fv_drawing.h>
#include <fv/fv_color.h>
#include <fv/fv_alloc.h>
#include <fv/fv_main.h>
#include <fv/fv_app.h>

/* This function will initalize app structure 
 * that is used in main function for managing
 * FVCode process */
fv_app_t* 
FV_CreateApp(i32 argc, char** argv)
{
    /* Allocate app strucutre using FV alloc function */
    fv_app_t* fv = FV_Calloc(1, sizeof(fv_app_t));

    fv->argc     = argc;
    fv->argv     = argv;
    fv->Init     = (fv_app_init_func)&FV_AppInitFunctionDefault;
    fv->Run      = (fv_app_run_func) &FV_AppRunFunctionDefault;

    return fv;
}

/* This function does destroy app and unallocate
 * every memory */
void
FV_DestroyApp(fv_app_t* app)
{
    /* Unallocate everything by using FV_UnallocAll */
    FV_UnallocAll();
    glfwTerminate();
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
int 
FV_AppInitFunctionDefault(fv_app_t* app)
{
    app->render = FV_RenderInit(app);
    FV_RenderCreateDefaultWindow(app->render);
    FV_RenderInitHandleViewportChange(app->render);
    FV_RenderInitGL(app->render);

    app->background = FV_NewColorRGB(28, 29, 29, 255);
    return 0;
}  

/* Function that will rn the app process.
 * Returns 0 on success, 1 on error */
int 
FV_AppRunFunctionDefault (fv_app_t* app)
{
    while (!FV_RenderShouldExit(app->render))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(FV_CONVERT_COLOR_TO_OPENGL(app->background));
        FV_DrawRenderQuad(app, FV_NewVector(150, 150), FV_NewVector(500, 300),
                          FV_NewColorRGB(123, 40, 150, 255), FV_NewColorRGB(255, 255, 150, 255), 5);
        // FV_DrawRenderLine(app, FV_NewVector(100, 100), FV_NewVector(400, 200),
        //                   FV_NewColorRGB(123, 40, 150, 255), 4);
        glfwSwapBuffers(app->render->window);
        glfwPollEvents();
    }

    FV_DestroyApp(app);
    return 0;
}