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
#include <fv/fv_string.h>
#include <fv/fv_drawing.h>

#include <fv/fv_font_manager.h>
#include <fv/fv_font_draw.h>

fv_code_area_lines_values_t
FV_ComponentCodeAreaGetValues(fv_component_code_area_t* code_area)
{
    if (splited_code == NULL)
        splited_code = FV_StringSplitByNewline(code_area->code_value);

    i32 lines_letters  = floor(log10(splited_code->length)) + 1;
    i32 left_padding   = round((lines_letters * code_area->font_size) * 1.3);
    i32 text_pos_start = code_area->pos.x + 12 + left_padding;

    return (fv_code_area_lines_values_t){
        lines_letters, left_padding, text_pos_start
    };
}

void
FV_ComponentTextBoxRenderLine(fv_component_t* component, fv_app_t* app, i32 line_index, 
                              fv_vector_t* line_position, fv_code_area_lines_values_t values)
{
    GET_CODE_AREA(component);

    char* current_line = FV_GetElementFromArray(splited_code, line_index);
    size_t current_line_len = strlen(current_line);

    for (i32 i = 0; i < (current_line_len == 0 ? 0 : (current_line_len + 1)); i++) 
    {
        if ((code_area->cursor->x == i && code_area->cursor->y == line_index) && code_area->focus)
            FV_DrawFillRect(app, FV_NewVector(line_position->x - 1, line_position->y + 2), 
                            FV_NewVector(2, code_area->font_size * 1.15), code_area->cursor_color);
        
        if (i != current_line_len + 1)
        {
            char textbox_value_char = current_line[i];

            if (textbox_value_char == '\n')
            {
                /* Newline character, advance y value and move x to the 
                 * starting point */
                line_position->y += code_area->font_size + code_area->line_space;
                line_position->x  = code_area->pos.x + 12 + values.left_padding;
            }
            else if (textbox_value_char == '\t')
            {
                /* Tab character, advance x by two times the 
                 * font size */
                line_position->x += code_area->font_size * 2;
            }
            else
            {
                SDL_Surface* glyph_surface = TTF_RenderGlyph_Blended(code_area->default_font->font, textbox_value_char,
                                        (SDL_Color){ code_area->foreground_color.r, code_area->foreground_color.g, 
                                                     code_area->foreground_color.b, code_area->foreground_color.a });
                SDL_Texture* glyph_texture = SDL_CreateTextureFromSurface(app->render->sdl_renderer, glyph_surface);

                fv_vector_t glyph_size = FV_NewVector(0, 0);
                i32 glyph_size_x, glyph_size_y;
                SDL_QueryTexture(glyph_texture, NULL, NULL, 
                                &glyph_size_x, &glyph_size_y);
                glyph_size.x = glyph_size_x;
                glyph_size.y = glyph_size_y;

                SDL_Rect rect = { 
                    .x = line_position->x, .y = line_position->y, 
                    .w = glyph_size.x,     .h = glyph_size.y
                };
                SDL_RenderCopy(app->render->sdl_renderer, glyph_texture, NULL, &rect);
                SDL_DestroyTexture(glyph_texture);
                SDL_FreeSurface(glyph_surface);
                line_position->x += glyph_size_x;
            }
        }
        
    }

    if (strlen(current_line) == 0 && ((code_area->cursor->x == 0 && code_area->cursor->y == line_index) && code_area->focus))
        FV_DrawFillRect(app, FV_NewVector(line_position->x - 1, line_position->y + 2), 
                        FV_NewVector(2, code_area->font_size * 1.15), code_area->cursor_color);

    line_position->x = code_area->pos.x + 12 + values.left_padding;
}

void
FV_ComponentCodeAreaRenderText(fv_component_t* component, fv_app_t* app)
{
    GET_CODE_AREA(component);

    splited_code = FV_StringSplitByNewline(code_area->code_value);

    fv_code_area_lines_values_t values = FV_ComponentCodeAreaGetValues(code_area);
    fv_vector_t line_position          = FV_NewVector(values.text_pos_start, code_area->pos.y + 5);

    for (i32 i = code_area->view_line; i < splited_code->length; i++)
    {
        if (line_position.y > (code_area->size.y + code_area->pos.y))
            break;

        FV_ComponentTextBoxRenderLine(component, app, i, &line_position, values);
        line_position.y += code_area->font_size + code_area->line_space;
    }

}

void 
FV_RenderLineNumberText(fv_app_t* app, fv_component_t* component, i32 size, 
                        fv_color_t fg, char* buffer, fv_code_area_lines_values_t values,
                        i32 number_position_y)
{
    GET_CODE_AREA(component);

    FV_SetFontSize(app->font_manager, FV_GetDefaultFont(app->font_manager), size);
    SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(code_area->default_font->font, buffer, (SDL_Color){ fg.r, fg.g, fg.b, fg.a }, 1280);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(app->render->sdl_renderer, surface);
    
    fv_vector_t font_size = FV_NewVector(0, 0);
    i32 font_size_x, font_size_y;
    SDL_QueryTexture(texture, NULL, NULL, 
                     &font_size_x, &font_size_y);

    font_size.x = (f32)font_size_x;
    font_size.y = (f32)font_size_y;

#if defined(FV_RIGHT_ALIGN_LINE_NUMBERS)
    SDL_Rect rect = { 
        .x = values.left_padding - font_size.x - 5, .y = number_position_y, 
        .w = font_size.x,                         .h = font_size.y
    };
#elif defined(FV_CENTER_ALIGN_LINE_NUMBERS)
    SDL_Rect rect = { 
        .x = values.left_padding / 2 - font_size.x / 2, .y = number_position_y, 
        .w = font_size.x,                         .h = font_size.y
    };
#endif

    SDL_RenderCopy(app->render->sdl_renderer, texture, NULL, &rect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void
FV_ComponentTextBoxRenderLineNumberText(fv_component_t* component, fv_app_t* app, i32 i, 
                                        fv_vector_t* number_position, fv_code_area_lines_values_t values)
{
    GET_CODE_AREA(component);

    /* Convert line number (i32) to char[32] */
    static char line_number_text_buffer[32] = { 0 };
    SDL_itoa(i, (char*)line_number_text_buffer, 10);

    /* Render this buffer */
    fv_color_t line_number_fg;
    if (code_area->cursor->y == i)
        line_number_fg = code_area->line_number_color;
    else 
        line_number_fg = code_area->darker_foreground_color;

    // FV_RenderFont(app, code_area->default_font, code_area->font_size, 1280, line_number_fg, 
                //   FV_NewVector(values.left_padding / 2, number_position->y), line_number_text_buffer);
    FV_RenderLineNumberText(app, component, code_area->font_size, line_number_fg, line_number_text_buffer,
                            values, number_position->y);
}

void
FV_ComponentTextBoxRenderLineNumbers(fv_component_t* component, fv_app_t* app)
{
    GET_CODE_AREA(component);

    fv_code_area_lines_values_t values = FV_ComponentCodeAreaGetValues(code_area);
    fv_vector_t number_position        = FV_NewVector(values.text_pos_start, code_area->pos.y + 5);

    FV_DrawFillRect(app, FV_NewVector(code_area->pos.x, code_area->pos.y), 
                    FV_NewVector(code_area->pos.x + values.left_padding, code_area->pos.y + code_area->size.y),
                    code_area->border_color);

    for (i32 i = code_area->view_line; i < splited_code->length; i++)
    {
        if (number_position.y > (code_area->size.y + code_area->pos.y))
            break;

        FV_ComponentTextBoxRenderLineNumberText(component, app, i, &number_position, values);
        number_position.y += code_area->font_size + code_area->line_space;
    }
}