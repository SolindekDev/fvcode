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

#ifndef __FV_COMPONENT_CODE_AREA_H
#define __FV_COMPONENT_CODE_AREA_H

#include <fv/fv.h>

#include <fv/fv_component_manager.h>
#include <fv/fv_component.h>

#include <fv/fv_font_manager.h>
#include <fv/fv_font_draw.h>

typedef struct __fv_code_area_lines_values {
    i32 lines_letters,
        left_padding,
        text_pos_start;
} fv_code_area_lines_values_t;

typedef struct __fv_code_area_highlight_t {
    i32 x;
} fv_code_area_highlight_t;

typedef struct {
    i32 x, y;
} fv_code_area_cursor_t;

typedef struct __fv_component_code_area_t {
    /* Position & size */
    fv_vector_t pos;
    fv_vector_t size;

    /* Color pallete, colors are defined in the 
     * config.h file*/
    fv_color_t darker_foreground_color;
    fv_color_t line_number_color;
    fv_color_t scroll_bar_color;
    fv_color_t background_color;
    fv_color_t foreground_color;
    fv_color_t highlight_color;
    fv_color_t border_color;
    fv_color_t cursor_color;

    /* Highlight, cursor and scroll properties */
    fv_code_area_highlight_t* highlight;
    fv_code_area_cursor_t   * cursor;

    /* line_space - Space in px between every line 
     * font_size  - Font size in px 
     * view_line  - First line number we see in our code_area  */
    f32 line_space;
    i32 font_size;
    i32 view_line;

    /* code_value - File context in code area
     * filename   - Name of edited right now file */
    char* code_value;
    char* filename;

    /* Default font, defined in the config.h file */
    fv_font_t* default_font;

    /* focus              - Is the code area selected
     * mouse_button_state - Current state of mouse */
    bool focus;
    bool mouse_button_state;

    fv_array_t* splited_code;
} fv_component_code_area_t;

#define GET_CODE_AREA(component)                                                        \
         fv_component_code_area_t* code_area = component->component_additional_data;    \

fv_component_t* FV_CreateComponentCodeArea(fv_vector_t pos, fv_vector_t size, i32 font_size, fv_font_t* font, char* code_value, char* filename);

i32 FV_ComponentCodeAreaRenderFunction(fv_component_t* component, fv_app_t* app);
i32 FV_ComponentCodeAreaEventFunction (fv_component_t* component, fv_app_t* app, SDL_Event event);
i32 FV_ComponentCodeAreaRunFunction   (fv_component_t* component, fv_app_t* app);

void FV_ComponentCodeAreaRenderText(fv_component_t* component, fv_app_t* app);
void FV_ComponentCodeAreaRenderLineNumbers(fv_component_t* component, fv_app_t* app);
void FV_ComponentCodeAreaSetCursorByMouse(fv_component_t* component, fv_app_t* app, SDL_Event event);

fv_code_area_lines_values_t FV_ComponentCodeAreaGetValues(fv_component_code_area_t* code_area);

/* This specific function will calculate position in text
 * by given mouse position. This function is widealy used in
 * fvcode so it needs to be as optimised as it can. */
fv_vector_t FV_CodeAreaPositionByMouse(fv_component_t* component, fv_vector_t mouse_position);

void FV_ComponentCodeAreaSetCursor(fv_component_t* component, fv_vector_t pos);
void FV_ComponentCodeAreaMoveUp   (fv_component_t* component);
void FV_ComponentCodeAreaMoveDown (fv_component_t* component);
void FV_ComponentCodeAreaMoveLeft (fv_component_t* component);
void FV_ComponentCodeAreaMoveRight(fv_component_t* component);

void FV_ComponentCodeAreaHandleMouseWheel(fv_component_t* component, SDL_Event event);
void FV_ComponentCodeAreaRenderScrollBar(fv_component_t* component, fv_app_t* app);
void FV_ComponentCodeAreaIncrementViewLine(fv_component_t* component);
void FV_ComponentCodeAreaDecrementViewLine(fv_component_t* component);

void FV_ComponentCodeAreaKeyDownEvent(fv_component_t* component, fv_app_t* app, SDL_Event event);
void FV_ComponentCodeAreaTextInput(fv_component_t* component, fv_app_t* app, SDL_Event event);

void FV_ComponentCodeAreaInsertCharAtCursor(fv_component_t* component, char append_char);

#endif /* __FV_COMPONENT_CODE_AREA_H */