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

#ifndef __FV_DRAWING_H
#define __FV_DRAWING_H

#include <fv/fv.h>

#include <fv/fv_vector.h>
#include <fv/fv_render.h>
#include <fv/fv_color.h>
#include <fv/fv_app.h>

void FV_DrawFillRect(fv_app_t* window, fv_vector_t pos, fv_vector_t size, fv_color_t color);
void FV_DrawRect(fv_app_t* window, fv_vector_t pos, fv_vector_t size, fv_color_t color);

void FV_DrawFillCircle(fv_app_t* window, fv_vector_t pos, u32 radius, fv_color_t color);
void FV_DrawCircle(fv_app_t* window, fv_vector_t pos, u32 radius, fv_color_t color);

void FV_DrawFillTriangle(fv_app_t* window, fv_vector_t point1, fv_vector_t point2, fv_vector_t point3, fv_color_t color);
void FV_DrawTriangle(fv_app_t* window, fv_vector_t point1, fv_vector_t point2, fv_vector_t point3, fv_color_t color);

void FV_DrawLine(fv_app_t* window, fv_vector_t start, fv_vector_t end, fv_color_t color);

#endif /* __FV_DRAWING_H */