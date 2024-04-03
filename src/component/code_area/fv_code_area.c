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
#include <config/config.h>

#include <fv/fv_component_code_area.h>

#include <fv/fv_component_lable.h>
#include <fv/fv_component_manager.h>
#include <fv/fv_component.h>
#include <fv/fv_collisions.h>
#include <fv/fv_drawing.h>
#include <fv/fv_string.h>

#include <fv/fv_font_manager.h>
#include <fv/fv_font_draw.h>

fv_component_t* 
FV_CreateComponentCodeArea(fv_vector_t pos, fv_vector_t size, i32 font_size, 
                           fv_font_t* font, char* code_value, char* filename)
{
    fv_component_t* code_area_component   = FV_CreateComponent("code_area", COMPONENT_CODE_AREA);
    code_area_component->component_render = FV_ComponentCodeAreaRenderFunction;
    code_area_component->component_event  = FV_ComponentCodeAreaEventFunction;
    code_area_component->component_run    = FV_ComponentCodeAreaRunFunction;

    fv_component_code_area_t* code_area = calloc(1, sizeof(fv_component_code_area_t));

    code_area_component->component_additional_data = code_area;

    code_area->pos          = pos;
    code_area->size         = size;
    code_area->font_size    = font_size;
    code_area->default_font = font;
    code_area->code_value   = code_value;
    code_area->filename     = filename;

    code_area->line_space = FV_CODE_AREA_LINE_SPACE;
    code_area->view_line  = 0;

    code_area->focus              = false;
    code_area->mouse_button_state = false;

    code_area->background_color         = FV_NewColorRGB(FV_CODE_AREA_BACKGROUND_COLOR        );
    code_area->foreground_color         = FV_NewColorRGB(FV_CODE_AREA_FOREGROUND_COLOR        );
    code_area->darker_foreground_color  = FV_NewColorRGB(FV_CODE_AREA_DARKER_FOREGROUND_COLOR );
    code_area->highlight_color          = FV_NewColorRGB(FV_CODE_AREA_HIGHLIGHT_COLOR         );
    code_area->border_color             = FV_NewColorRGB(FV_CODE_AREA_BORDER_COLOR            );
    code_area->cursor_color             = FV_NewColorRGB(FV_CODE_AREA_CURSOR_COLOR            );
    code_area->line_number_color        = FV_NewColorRGB(FV_CODE_AREA_LINE_NUMBER_COLOR       );

    code_area->highlight = calloc(1, sizeof(fv_code_area_highlight_t));
    code_area->cursor    = calloc(1, sizeof(fv_code_area_cursor_t   ));

    return code_area_component;
}

i32 
FV_ComponentCodeAreaRenderFunction(fv_component_t* component, fv_app_t* app)
{
    GET_CODE_AREA(component);

    FV_DrawFillRect(app, code_area->pos, code_area->size, code_area->background_color);

    FV_ComponentCodeAreaRenderText(component, app);
    FV_ComponentCodeAreaRenderLineNumbers(component, app);
    FV_ComponentCodeAreaRenderScrollBar(component, app)

    return 0;
}   

i32 
FV_ComponentCodeAreaRunFunction(fv_component_t* component, fv_app_t* app)
{
    GET_CODE_AREA(component);
    code_area->splited_code = FV_StringSplitByNewline(code_area->code_value);

    return 0;
}