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

#include <SDL2/SDL.h> 

fv_render_t*
FV_RenderInit(fv_app_t* parent_app)
{
    FV_SUCCESS("Initalizing render", 0);

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        FV_ERROR("Couldn't initialize SDL2 library: \'%s\'", SDL_GetError());

    fv_render_t* render = FV_Calloc(1, sizeof(fv_render_t));
    render->app  = parent_app;
    render->exit = 0;
    return render;
}

void
FV_RenderCreateDefaultWindow(fv_render_t* render)
{
    FV_SUCCESS("Creating a window", 0);

    const u32 renderer_flags = FV_RENDERER_FLAGS;
    const u32 window_flags   = FV_WINDOW_FLAGS;

    const i32 window_x     = FV_WINDOW_SIZE_X;
    const i32 window_y     = FV_WINDOW_SIZE_Y;

    render->sdl_window = SDL_CreateWindow(FV_WINDOW_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                          window_x, window_y, window_flags);
    if (render->sdl_window == NULL)
        FV_ERROR("Function \'SDL_CreateWindow\' failed, Couldn't create a window: \'%s\'", SDL_GetError());

    render->sdl_renderer = SDL_CreateRenderer(render->sdl_window, -1, renderer_flags);
    if (render->sdl_renderer == NULL)
        FV_ERROR("Function \'SDL_CreateRenderer\' failed, Couldn't create a renderer: \'%s\'", SDL_GetError());
}

bool
FV_RenderShouldExit(fv_render_t* render)
{
    return !render->exit;
}

void
FV_RenderCatchEvents(fv_render_t* render)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) 
        switch (event.type) 
        {
            case SDL_QUIT:
                render->exit = 1;
                break;
        }
}

void
FV_RenderClearWindow(fv_render_t* render)
{
    SDL_SetRenderDrawColor(render->sdl_renderer, render->app->background.r, 
                           render->app->background.g, render->app->background.b, render->app->background.a);
    SDL_RenderClear(render->sdl_renderer);
}

void
FV_RenderSwapBuffer(fv_render_t* render)
{
    SDL_RenderPresent(render->sdl_renderer);
}