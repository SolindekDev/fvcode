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

#ifndef __FV_RENDER_H
#define __FV_RENDER_H

#include <fv/fv.h>
#include <fv/fv_app.h>

#include <SDL2/SDL.h> 

typedef struct __fv_app_t fv_app_t;

typedef struct __fv_render_t {
	SDL_Renderer* sdl_renderer;
	SDL_Window*   sdl_window;
    fv_app_t*     app;
    bool          exit;

    u64     frames;
    u64     fps;
    clock_t last_tick; 
} fv_render_t;

fv_render_t* FV_RenderInit(fv_app_t* parent_app);

void FV_RenderCreateDefaultWindow(fv_render_t* render);
bool FV_RenderShouldExit(fv_render_t* render);

void FV_RenderCatchEvents(fv_render_t* render);
void FV_RenderSwapBuffer (fv_render_t* render);
void FV_RenderClearWindow(fv_render_t* render);

#endif /* __FV_RENDER_H */