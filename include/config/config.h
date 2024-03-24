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
#define FV_WINDOW_SIZE_Y  920

#define FV_WINDOW_FLAGS   (SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE)
#define FV_RENDERER_FLAGS (SDL_RENDERER_ACCELERATED)

#define FV_MAX_FORMAT_BUFFER (i32)(1024 * 4)

/* Font */
#define FV_DEFAULT_FONT_SIZE 16
#define FV_DEFAULT_FONT_PATH "./fonts/jetbrains/JetBrainsMono-Regular.ttf"

/* Code area color pallet */
#define FV_CODE_AREA_HIGHLIGHT_COLOR  129, 125, 255, 120
#define FV_CODE_AREA_CURSOR_COLOR     94,  161, 242, 240
#define FV_CODE_AREA_BORDER_COLOR     26,  26,  26,  255
#define FV_CODE_AREA_BACKGROUND_COLOR 19,  19,  19,  255
#define FV_CODE_AREA_FOREGROUND_COLOR 211, 215, 207, 255

/* Code area line space */
#define FV_CODE_AREA_LINE_SPACE 7.0f

#ifdef __APPLE__
# define FV_CONFIG_CONTROL_MOD (KMOD_LGUI | KMOD_RGUI)
#else
# define FV_CONFIG_CONTROL_MOD (KMOD_LCTRL | KMOD_RCTRL)
#endif

#define FV_BACKGROUND_COLOR 28, 29, 29, 255

#endif /* __FV_CONFIG_H */