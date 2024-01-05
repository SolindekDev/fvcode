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

#include <fv/fv_render.h>
#include <fv/fv_msg.h>
#include <fv/fv_alloc.h>
#include <fv/fv_app.h>

#include <GLFW/glfw3.h>

fv_render_t*
FV_RenderInit(fv_app_t* parent_app)
{
    FV_SUCCESS("Initalizing render", 0);

    if (glfwInit() == false)
        FV_ERROR("Couldn't initialize GLFW library.", 0);

    fv_render_t* render = FV_Calloc(1, sizeof(fv_render_t));
    return render;
}

void
FV_RenderCreateDefaultWindow(fv_render_t* render)
{
    FV_SUCCESS("Creating a window", 0);

    const int window_x = FV_WINDOW_SIZE_X;
    const int window_y = FV_WINDOW_SIZE_Y;

    render->window = glfwCreateWindow(window_x, window_y, FV_WINDOW_NAME, NULL, NULL);

    if (render->window == NULL)
    {
        FV_ERROR_NO_EXIT("\'glfwCreateWindow\' failed returning NULL.", 0);
        FV_DestroyAppAndExit(render->app, 1);
    }

    glfwMakeContextCurrent(render->window);
}

bool
FV_RenderShouldExit(fv_render_t* render)
{
    return glfwWindowShouldClose(render->window);
}

void
FV_RenderHandleViewportChange(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void
FV_RenderInitHandleViewportChange(fv_render_t* render)
{
    glfwSetFramebufferSizeCallback(render->window, FV_RenderHandleViewportChange);
}

void
FV_RenderInitGL(fv_render_t* render)
{
    FV_SUCCESS("Initializing OpenGL", 0);

    /* This code makes drawing lines more smooth */
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    /* Set the viewport */
    glViewport(0, 0, FV_WINDOW_SIZE_X, FV_WINDOW_SIZE_Y);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, FV_WINDOW_SIZE_X, FV_WINDOW_SIZE_Y, 0, -1, 1); // Adjust the parameters based on your needs

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}