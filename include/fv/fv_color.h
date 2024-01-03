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

#ifndef __FV_COLOR_H
#define __FV_COLOR_H

#include <fv/fv.h>

#define FV_CONVERT_COLOR_TO_OPENGL(color)                 \
    ((float)color.r / 255.0f), ((float)color.g / 255.0f), \
    ((float)color.b / 255.0f), ((float)color.a / 255.0f)  \

typedef struct __fv_color_t {
    float r, g, b, a;
} fv_color_t;

fv_color_t FV_NewColorRGB(float r, float g, float b, float a);
fv_color_t FV_NewColorHSV(float h, float s, float v, float a);
fv_color_t FV_NewColorHex(u64 hex);

#endif /* __FV_COLOR_H */