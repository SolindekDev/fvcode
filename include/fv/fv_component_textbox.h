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

#ifndef __FV_COMPONENT_TEXTBOX_H
#define __FV_COMPONENT_TEXTBOX_H

#include <fv/fv.h>

#include <fv/fv_component_manager.h>
#include <fv/fv_component.h>

#include <fv/fv_font_manager.h>
#include <fv/fv_font_draw.h>

typedef struct __fv_component_textbox_t {
    fv_vector_t pos;
    fv_color_t  border_color;
    fv_color_t  cursor_color;
    fv_color_t  bg;
    fv_color_t  fg;
    float       line_space;
    char*       textbox_value;
    bool        disable_writting;
    fv_font_t*  font;
    i32         font_size;
    bool        focus;
    fv_vector_t cursor;
    fv_array_t* textbox_lines;
    fv_vector_t size;
} fv_component_textbox_t;

fv_component_t* FV_CreateComponentTextBox(fv_vector_t pos, fv_vector_t size, fv_color_t bg, fv_color_t fg, char* textbox_value, fv_font_t* font, i32 font_size, fv_color_t border_color, float line_space);

int FV_ComponentTextBoxRenderFunction(fv_component_t* component, fv_app_t* app);
int FV_ComponentTextBoxEventFunction (fv_component_t* component, fv_app_t* app, SDL_Event event);
int FV_ComponentTextBoxRunFunction   (fv_component_t* component, fv_app_t* app);

#endif /* __FV_COMPONENT_TEXTBOX_H */