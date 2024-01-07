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

typedef struct __fv_color_t {
    u8 r, g, b, a;
} fv_color_t;

fv_color_t FV_NewColorRGB(u8 r, u8 g, u8 b, u8 a);
fv_color_t FV_NewColorHSV(u8 h, u8 s, u8 v, u8 a);
fv_color_t FV_NewColorHex(u64 hex);

#endif /* __FV_COLOR_H */