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

#ifndef __FV_FONT_DRAW_H
#define __FV_FONT_DRAW_H

#include <fv/fv.h>

#include <fv/fv_font_manager.h>
#include <fv/fv_vector.h>
#include <fv/fv_color.h>
#include <fv/fv_app.h>

fv_vector_t FV_RenderFont(fv_app_t* app, fv_font_t* font, i32 size, i32 newline, fv_color_t color, 
                          fv_vector_t pos, const char* msg);

fv_vector_t FV_RenderFontFormat(fv_app_t* app, fv_font_t* font, i32 size, i32 newline, fv_color_t color, 
                                fv_vector_t pos, const char* fmt, ...);

#endif /* __FV_FONT_DRAW_H */