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

#ifndef __FV_CONFIG_H
#define __FV_CONFIG_H

#include <stdbool.h>
#include <SDL2/SDL.h>

/* Version */
#define FV_VERSION     "0.1.0v BETA"

/* Window */
#define FV_WINDOW_NAME   "FVCode " FV_VERSION

#define FV_WINDOW_SIZE_X  1280
#define FV_WINDOW_SIZE_Y  720

#define FV_WINDOW_FLAGS   (SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_SHOWN)
#define FV_RENDERER_FLAGS (SDL_RENDERER_ACCELERATED)

#define FV_MAX_FORMAT_BUFFER (int)(1024 * 3)

#endif /* __FV_CONFIG_H */