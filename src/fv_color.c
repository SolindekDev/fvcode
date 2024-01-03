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

#include <fv/fv_color.h>

/* Create color in RGBA standard */
fv_color_t 
FV_NewColorRGB(float r, float g, float b, float a)
{
    return ((fv_color_t){ r, g, b, a });
}

/* Create color in HSV standard */
fv_color_t 
FV_NewColorHSV(float h, float s, float v, float a)
{
    /* TODO: Algorithm for converting HSV color to RGB */
    
    return FV_NewColorRGB(215, 105, 132, a);
}

/* Create color in RGBA by hexadecimal value*/
fv_color_t 
FV_NewColorHex(u64 hex)
{
    fv_color_t hex_color = FV_NewColorRGB(0, 0, 0, 255);

    hex_color.r = (hex >> 24) & 0xFF;
    hex_color.g = (hex >> 16) & 0xFF;
    hex_color.b = (hex >>  8) & 0xFF;
    hex_color.a = (   hex   ) & 0xFF;  

    return hex_color;
}
