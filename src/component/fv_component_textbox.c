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

#include <fv/fv_component_textbox.h>
#include <fv/fv_component_manager.h>
#include <fv/fv_component.h>

#include <fv/fv_font_manager.h>
#include <fv/fv_font_draw.h>

#include <fv/fv_collisions.h>
#include <fv/fv_drawing.h>
#include <fv/fv_string.h>
#include <fv/fv_math.h>

#include <fv/fv_msg.h>

static char info_box_fmt[FV_MAX_FORMAT_BUFFER] = { 0 };
static char rest_of_line[MAX_LINE_LENGTH     ] = { 0 };
static i32  glyph_size_                        = 0;

fv_component_t*
FV_CreateComponentTextBox(fv_vector_t pos, fv_vector_t size, fv_color_t bg, fv_color_t fg, char* textbox_value, 
                          fv_font_t* font, i32 font_size, fv_color_t border_color, float line_space, char* filename,
                          char* path)
{   
    fv_component_t* textbox_component   = FV_CreateComponent("textbox", COMPONENT_TEXTBOX);
    textbox_component->component_render = FV_ComponentTextBoxRenderFunction;
    textbox_component->component_event  = FV_ComponentTextBoxEventFunction;
    textbox_component->component_run    = FV_ComponentTextBoxRunFunction;

    fv_component_textbox_t* textbox = calloc(1, sizeof(fv_component_textbox_t));
    textbox->pos              = pos;
    textbox->bg               = bg;
    textbox->fg               = fg;
    textbox->border_color     = border_color;
    textbox->font             = font;
    textbox->font_size        = font_size;
    textbox->disable_writting = false;
    textbox->line_space       = line_space;
    textbox->cursor_color     = FV_NewColorRGB(FV_DEFAULT_CURSOR_COLOR);
    textbox->size             = size;
    textbox->filename         = filename;
    textbox->path             = path;
    textbox->view_line_start  = 0;

    textbox->textbox_value = calloc(strlen(textbox_value) + 1, sizeof(char));
    memcpy(textbox->textbox_value, textbox_value, strlen(textbox_value) + 1);

    textbox->textbox_lines = FV_StringSplitByNewline(textbox->textbox_value);

    textbox_component->component_additional_data = textbox;
    return textbox_component;
}

void
FV_ComponentTextBoxDisableHighlight(fv_component_t* component, fv_app_t* app)
{
    fv_component_textbox_t* textbox = component->component_additional_data;

    textbox->highlight_multiply_lines  = false;
    textbox->highlight_start_pos       = FV_NewVector(0, 0);
    textbox->highlight_end_pos         = FV_NewVector(0, 0);
    textbox->highlight_size            = FV_NewVector(0, 0);
    textbox->highlight_pos             = FV_NewVector(0, 0);
    textbox->highlight                 = false;
}

bool
FV_ComponentTextBoxIsHighlight(fv_component_t* component, fv_app_t* app)
{
    fv_component_textbox_t* textbox = component->component_additional_data;

    // TODO: FV_ComponentTextBoxIsHighlight
}

i32
FV_ComponentTextBoxGetLinesToSee(fv_component_t* component, fv_app_t* app)
{
    fv_component_textbox_t* textbox = component->component_additional_data;

    i32 line_pos_y = textbox->pos.y;
    i32 index      = 0;

    for (index = 0; index < textbox->textbox_lines->length; index++)
    {
        if (line_pos_y > (textbox->size.y + textbox->pos.y))
            break;

        line_pos_y += textbox->font_size + textbox->line_space;
    }

    return index - 2;
}

void
FV_ComponentTextBoxDecrementCursorY(fv_component_t* component, fv_app_t* app)
{
    fv_component_textbox_t* textbox = component->component_additional_data;

    if (textbox->cursor.y != 0)
        textbox->cursor.y--;

    if (textbox->cursor.y < textbox->view_line_start)
        textbox->view_line_start--;
}

void
FV_ComponentTextBoxIncrementCursorY(fv_component_t* component, fv_app_t* app)
{
    fv_component_textbox_t* textbox = component->component_additional_data;

    if (textbox->cursor.y != textbox->textbox_lines->length)
        textbox->cursor.y++;

    if (textbox->cursor.y == (textbox->view_line_start + FV_ComponentTextBoxGetLinesToSee(component, app)))
        textbox->view_line_start++;
}

void
FV_ComponentTextBoxIncrementViewLine(fv_component_t* component, fv_app_t* app)
{
    fv_component_textbox_t* textbox = component->component_additional_data;

    if (textbox->view_line_start != (textbox->textbox_lines->length - 1))
        textbox->view_line_start++;
}

void
FV_ComponentTextBoxDecrementViewLine(fv_component_t* component, fv_app_t* app)
{
    fv_component_textbox_t* textbox = component->component_additional_data;

    if (textbox->view_line_start != 0)
        textbox->view_line_start--;
}

void
FV_ComponentTextBoxRenderLine(fv_component_t* component, fv_app_t* app, 
                              i32 line_index, fv_vector_t* character_vector_pos, i32 left_padding)
{
    fv_component_textbox_t* textbox = component->component_additional_data;
    char* current_line = FV_GetElementFromArray(textbox->textbox_lines, line_index);
    size_t current_line_len = strlen(current_line);

    for (int i = 0; i < (current_line_len == 0 ? 0 : (current_line_len + 1)); i++) 
    {
        if ((textbox->cursor.x == i && textbox->cursor.y == line_index) && textbox->focus)
        {
            FV_DrawFillRect(app, FV_NewVector(character_vector_pos->x - 1, character_vector_pos->y + 2), 
                            FV_NewVector(2, textbox->font_size * 1.15), textbox->cursor_color);
        }

        if (i != current_line_len + 1)
        {
            char textbox_value_char = current_line[i];

            if (textbox_value_char == '\n')
            {
                character_vector_pos->y += textbox->font_size + textbox->line_space;
                character_vector_pos->x  = textbox->pos.x + 12 + left_padding;
            }
            else if (textbox_value_char == '\t')
            {
                character_vector_pos->x += (textbox->font_size / 2) * 4;
            }
            else
            {
                SDL_Surface* glyph_surface = TTF_RenderGlyph_Blended(textbox->font->font, textbox_value_char,
                                        (SDL_Color){ textbox->fg.r, textbox->fg.g, textbox->fg.b, textbox->fg.a });
                SDL_Texture* glyph_texture = SDL_CreateTextureFromSurface(app->render->sdl_renderer, glyph_surface);

                fv_vector_t glyph_size = FV_NewVector(0, 0);
                i32 glyph_size_x, glyph_size_y;
                SDL_QueryTexture(glyph_texture, NULL, NULL, 
                                &glyph_size_x, &glyph_size_y);
                glyph_size.x = glyph_size_x;
                glyph_size.y = glyph_size_y;

                SDL_Rect rect = { 
                    .x = character_vector_pos->x, .y = character_vector_pos->y, 
                    .w = glyph_size.x,           .h = glyph_size.y
                };
                SDL_RenderCopy(app->render->sdl_renderer, glyph_texture, NULL, &rect);
                SDL_DestroyTexture(glyph_texture);
                SDL_FreeSurface(glyph_surface);
                glyph_size_ = glyph_size_x + 1;
                character_vector_pos->x += glyph_size_x + 1;
            }
        }
    }

    if (strlen(current_line) == 0 && ((textbox->cursor.x == 0 && textbox->cursor.y == line_index) && textbox->focus))
    {
        FV_DrawFillRect(app, FV_NewVector(character_vector_pos->x - 1, character_vector_pos->y + 2), 
                        FV_NewVector(2, textbox->font_size * 1.15), textbox->cursor_color);
    }

    character_vector_pos->x = textbox->pos.x + 12 + left_padding;
}

void
FV_ComponentTextBoxRenderLineNumbers(fv_component_t* component, fv_app_t* app)
{
    fv_component_textbox_t* textbox = component->component_additional_data;
    i32 lines_letters = floor(log10(textbox->textbox_lines->length)) + 1;
    i32 left_padding = round((lines_letters * textbox->font_size) * 1.3);
    fv_vector_t line_vector_pos = FV_NewVector(textbox->pos.x + 10, textbox->pos.y);

    FV_DrawFillRect(app, FV_NewVector(textbox->pos.x, textbox->pos.y), 
                    FV_NewVector(textbox->pos.x + left_padding, textbox->pos.y + textbox->size.y),
                    textbox->border_color);

    char line_num_buffer[64] = { 0 };
    FV_SetFontSize(app->font_manager, textbox->font, textbox->font_size);
    for (int i = textbox->view_line_start; i < textbox->textbox_lines->length; i++)
    {
        if (line_vector_pos.y > (textbox->size.y + textbox->pos.y))
            break;

        SDL_itoa(i, (char*)line_num_buffer, 10);
        
        SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(textbox->font->font, (char*)line_num_buffer, (SDL_Color){ textbox->fg.r, textbox->fg.g, textbox->fg.b, textbox->fg.a }, line_vector_pos.x + left_padding);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(app->render->sdl_renderer, surface);

        fv_vector_t font_size = FV_NewVector(0, 0);
        i32 font_size_x, font_size_y;
        SDL_QueryTexture(texture, NULL, NULL, 
                        &font_size_x, &font_size_y);

        font_size.x = font_size_x;
        font_size.y = font_size_y;

        SDL_Rect rect = { 
            .x = left_padding / 2 - font_size.x / 2, .y = line_vector_pos.y, 
            .w = font_size.x,                        .h = font_size.y
        };
        SDL_RenderCopy(app->render->sdl_renderer, texture, NULL, &rect);

        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
        memset(line_num_buffer, 0, 64);
        line_vector_pos.y += textbox->font_size + textbox->line_space;
        line_vector_pos.x  = textbox->pos.x + 8;
    }
}

void
FV_ComponentTextBoxRenderText(fv_component_t* component, fv_app_t* app)
{
    fv_component_textbox_t* textbox = component->component_additional_data;

    i32 lines_letters = floor(log10(textbox->textbox_lines->length)) + 1;
    i32 left_padding = round((lines_letters * textbox->font_size) * 1.3);
    fv_vector_t character_vector_pos = FV_NewVector(textbox->pos.x + 12 + left_padding, textbox->pos.y);

    for (int i = textbox->view_line_start; i < textbox->textbox_lines->length; i++)
    {
        if (character_vector_pos.y > (textbox->size.y + textbox->pos.y))
            break;

        FV_ComponentTextBoxRenderLine(component, app, i, &character_vector_pos, left_padding);
        character_vector_pos.y += textbox->font_size + textbox->line_space;
    }
}

void
FV_ComponentTextBoxRenderInfo(fv_component_t* component, fv_app_t* app)
{
    fv_component_textbox_t* textbox = component->component_additional_data;

    sprintf((char*)info_box_fmt, "Total lines: %zu, Cursor: %d:%d, Path: \"%s\"",
        textbox->textbox_lines->length, (i32)textbox->cursor.x, (i32)textbox->cursor.y, textbox->path);

    i32 lines_letters = floor(log10(textbox->textbox_lines->length)) + 1;
    i32 left_padding = round((lines_letters * textbox->font_size) * 1.3);
    fv_vector_t pos = FV_NewVector(textbox->pos.x, textbox->size.y - (textbox->font_size) - 10);

    FV_DrawFillRect(app, pos, FV_NewVector(textbox->size.x - pos.x, (textbox->font_size) + 10), 
                    textbox->border_color);
    FV_SetFontSize(app->font_manager, textbox->font, textbox->font_size);
    SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(textbox->font->font, info_box_fmt, (SDL_Color){ 199, 199, 199, 255 }, 1280);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(app->render->sdl_renderer, surface);
    memset(info_box_fmt, 0, FV_MAX_FORMAT_BUFFER);

    fv_vector_t font_size = FV_NewVector(0, 0);
    i32 font_size_x, font_size_y;
    SDL_QueryTexture(texture, NULL, NULL,  
                     &font_size_x, &font_size_y);

    font_size.x = font_size_x;
    font_size.y = font_size_y;

    SDL_Rect rect = { 
        .x = textbox->size.x - font_size.x - 10, .y = pos.y + 3, 
        .w = font_size.x,                        .h = font_size.y
    };
    SDL_RenderCopy(app->render->sdl_renderer, texture, NULL, &rect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    FV_SetFontSize(app->font_manager, textbox->font, textbox->font_size);
}

void
FV_ComponentTextBoxRenderScrollBar(fv_component_t* component, fv_app_t* app)
{
    fv_component_textbox_t* textbox = component->component_additional_data;

    // fv_color_t scrollbar_color = FV_NewColorRGB(textbox->border_color.r + 10, textbox->border_color.g + 10, 
    //                                             textbox->border_color.b + 10, 255                          );
    // FV_DrawFillRect(app, FV_NewVector((textbox->pos.x + textbox->size.x) - 16, textbox->pos.y),
    //                 FV_NewVector(16, textbox->size.y - (textbox->font_size) - 10), scrollbar_color);

}

void
FV_ComponentTextBoxRenderHighlight(fv_component_t* component, fv_app_t* app)
{
    fv_component_textbox_t* textbox = component->component_additional_data;

    i32 lines_letters  = floor(log10(textbox->textbox_lines->length)) + 1;
    i32 left_padding   = round((lines_letters * textbox->font_size) * 1.3);
    i32 text_pos_start = textbox->pos.x + 12 + left_padding;

    // if (textbox->highlight_start_pos.y == textbox->highlight_end_pos.y)
    // {
    //     textbox->highlight_multiply_lines = false;
    //     textbox->highlight_pos  = textbox->highlight_start_pos;
    //     textbox->highlight_size = FV_NewVector(
    //         FV_MAX(textbox->highlight_start_pos.x, textbox->highlight_end_pos.x) -
    //         FV_MIN(textbox->highlight_start_pos.x, textbox->highlight_end_pos.x), 
    //         textbox->highlight_start_pos.y);
    // }

    if (textbox->highlight_multiply_lines)
    {
        i32 row_difference = FV_MAX(textbox->highlight_start_pos.y, textbox->highlight_end_pos.y) -
                             FV_MIN(textbox->highlight_start_pos.y, textbox->highlight_end_pos.y);
        if (row_difference == 1)
        {
            if (textbox->highlight_start_pos.y < textbox->highlight_end_pos.y)
            {
                fv_vector_t first_line_highlight_pos   = FV_NewVector(text_pos_start + (textbox->highlight_start_pos.x * glyph_size_),
                                                                    textbox->pos.y + (textbox->highlight_start_pos.y * (textbox->font_size + textbox->line_space)));
                fv_vector_t first_line_hightlight_size = FV_NewVector(
                                                            (strlen(FV_GetElementFromArray(textbox->textbox_lines, textbox->highlight_start_pos.y + textbox->view_line_start)) - textbox->highlight_start_pos.x) * glyph_size_, 
                                                            textbox->font_size + textbox->line_space);
                FV_DrawFillRect(app, first_line_highlight_pos, first_line_hightlight_size, FV_NewColorRGB(FV_HIGHLIGHT_COLOR));
                fv_vector_t last_line_highlight_pos   = FV_NewVector(text_pos_start,
                                                                    textbox->pos.y + (textbox->highlight_end_pos.y * (textbox->font_size + textbox->line_space)));
                fv_vector_t last_line_hightlight_size = FV_NewVector(
                                                            (textbox->highlight_end_pos.x) * glyph_size_, 
                                                            textbox->font_size + textbox->line_space);
                FV_DrawFillRect(app, last_line_highlight_pos, last_line_hightlight_size, FV_NewColorRGB(FV_HIGHLIGHT_COLOR));
            }
            else
            {
                fv_vector_t first_line_highlight_pos   = FV_NewVector(text_pos_start, textbox->pos.y + (textbox->highlight_start_pos.y * (textbox->font_size + textbox->line_space)));
                fv_vector_t first_line_hightlight_size = FV_NewVector((textbox->highlight_start_pos.x) * glyph_size_, textbox->font_size + textbox->line_space);
                FV_DrawFillRect(app, first_line_highlight_pos, first_line_hightlight_size, FV_NewColorRGB(FV_HIGHLIGHT_COLOR));
                fv_vector_t last_line_highlight_pos   = FV_NewVector(text_pos_start + (textbox->highlight_end_pos.x * glyph_size_),
                                                                    textbox->pos.y + (textbox->highlight_end_pos.y * (textbox->font_size + textbox->line_space)));
                fv_vector_t last_line_hightlight_size = FV_NewVector(
                                                            (strlen(FV_GetElementFromArray(textbox->textbox_lines, textbox->highlight_end_pos.y + textbox->view_line_start)) - textbox->highlight_end_pos.x) * glyph_size_, 
                                                            textbox->font_size + textbox->line_space);
                FV_DrawFillRect(app, last_line_highlight_pos, last_line_hightlight_size, FV_NewColorRGB(FV_HIGHLIGHT_COLOR));
            }
        }
        else if (row_difference == 0)
            FV_DrawFillRect(app, textbox->highlight_pos, textbox->highlight_size, FV_NewColorRGB(FV_HIGHLIGHT_COLOR));
        else
        {
            if (textbox->highlight_start_pos.y < textbox->highlight_end_pos.y)
            {
                fv_vector_t first_line_highlight_pos   = FV_NewVector(text_pos_start + (textbox->highlight_start_pos.x * glyph_size_),
                                                                    textbox->pos.y + (textbox->highlight_start_pos.y * (textbox->font_size + textbox->line_space)));
                fv_vector_t first_line_hightlight_size = FV_NewVector(
                                                            (strlen(FV_GetElementFromArray(textbox->textbox_lines, textbox->highlight_start_pos.y + textbox->view_line_start)) - textbox->highlight_start_pos.x) * glyph_size_, 
                                                            textbox->font_size + textbox->line_space);
                FV_DrawFillRect(app, first_line_highlight_pos, first_line_hightlight_size, FV_NewColorRGB(FV_HIGHLIGHT_COLOR));
                fv_vector_t last_line_highlight_pos   = FV_NewVector(text_pos_start,
                                                                    textbox->pos.y + (textbox->highlight_end_pos.y * (textbox->font_size + textbox->line_space)));
                fv_vector_t last_line_hightlight_size = FV_NewVector(
                                                            (textbox->highlight_end_pos.x) * glyph_size_, 
                                                            textbox->font_size + textbox->line_space);
                FV_DrawFillRect(app, last_line_highlight_pos, last_line_hightlight_size, FV_NewColorRGB(FV_HIGHLIGHT_COLOR));
                for (int i = 1; i < row_difference; i++)
                {
                    FV_DrawFillRect(app, FV_NewVector(text_pos_start, textbox->pos.y + (textbox->highlight_start_pos.y + i) * (textbox->font_size + textbox->line_space)),
                                    FV_NewVector(strlen(FV_GetElementFromArray(textbox->textbox_lines, textbox->highlight_start_pos.y + textbox->view_line_start + i)) * glyph_size_, textbox->font_size + textbox->line_space),
                                    FV_NewColorRGB(FV_HIGHLIGHT_COLOR));
                }
            }
            else
            {
                fv_vector_t first_line_highlight_pos   = FV_NewVector(text_pos_start, textbox->pos.y + (textbox->highlight_start_pos.y * (textbox->font_size + textbox->line_space)));
                fv_vector_t first_line_hightlight_size = FV_NewVector((textbox->highlight_start_pos.x) * glyph_size_, textbox->font_size + textbox->line_space);
                FV_DrawFillRect(app, first_line_highlight_pos, first_line_hightlight_size, FV_NewColorRGB(FV_HIGHLIGHT_COLOR));
                fv_vector_t last_line_highlight_pos   = FV_NewVector(text_pos_start + (textbox->highlight_end_pos.x * glyph_size_),
                                                                    textbox->pos.y + (textbox->highlight_end_pos.y * (textbox->font_size + textbox->line_space)));
                fv_vector_t last_line_hightlight_size = FV_NewVector(
                                                            (strlen(FV_GetElementFromArray(textbox->textbox_lines, textbox->highlight_end_pos.y + textbox->view_line_start)) - textbox->highlight_end_pos.x) * glyph_size_, 
                                                            textbox->font_size + textbox->line_space);
                FV_DrawFillRect(app, last_line_highlight_pos, last_line_hightlight_size, FV_NewColorRGB(FV_HIGHLIGHT_COLOR));
                for (int i = 1; i < row_difference; i++)
                {
                    FV_DrawFillRect(app, FV_NewVector(text_pos_start, textbox->pos.y + (textbox->highlight_end_pos.y + i) * (textbox->font_size + textbox->line_space)),
                                    FV_NewVector(strlen(FV_GetElementFromArray(textbox->textbox_lines, textbox->highlight_end_pos.y + textbox->view_line_start + i)) * glyph_size_, textbox->font_size + textbox->line_space),
                                    FV_NewColorRGB(FV_HIGHLIGHT_COLOR));
                }
            }
        }
    }
    else
        FV_DrawFillRect(app, textbox->highlight_pos, textbox->highlight_size, FV_NewColorRGB(FV_HIGHLIGHT_COLOR));
}

int 
FV_ComponentTextBoxRenderFunction(fv_component_t* component, fv_app_t* app)
{
    fv_component_textbox_t* textbox = component->component_additional_data;

    FV_DrawFillRect(app, textbox->pos, textbox->size, textbox->bg);
    FV_DrawRect(app, textbox->pos, textbox->size, textbox->border_color);
    // FV_RenderFont(app, textbox->font, textbox->font_size, textbox->size.x + textbox->pos.x, textbox->fg, FV_NewVector(textbox->pos.x + 7, textbox->pos.y), textbox->textbox_value);
    
    FV_ComponentTextBoxRenderText       (component, app);
    FV_ComponentTextBoxRenderLineNumbers(component, app);
    FV_ComponentTextBoxRenderHighlight  (component, app);
    FV_ComponentTextBoxRenderInfo       (component, app);
    FV_ComponentTextBoxRenderScrollBar  (component, app);

    return 0;
}

void
FV_ComponentTextBoxBackspaceMultilineHighlight(fv_component_t* component, fv_app_t* app, 
                                               fv_component_textbox_t* textbox)
{
    // FIXME: When deleting more than 2 lines after scrolling functions 
    //        doesnt delete some lines
    FV_FIXME("This functions is not done and it has some major bugs in it.", 0);

    i32 lines_letters  = floor(log10(textbox->textbox_lines->length)) + 1;
    i32 left_padding   = round((lines_letters * textbox->font_size) * 1.3);
    i32 text_pos_start = textbox->pos.x + 12 + left_padding;

    i32 start_pos_y = (i32)(FV_MIN(textbox->highlight_start_pos.y + textbox->view_line_start, textbox->highlight_end_pos.y + textbox->view_line_start));
    i32 end_pos_y   = (i32)(FV_MAX(textbox->highlight_start_pos.y + textbox->view_line_start, textbox->highlight_end_pos.y + textbox->view_line_start));
    
    i32 highlighted_lines = end_pos_y - start_pos_y;

    char* first_line;
    char*  last_line;
    i32 first_len, first_start, first_end, first_y;
    i32  last_len,  last_start,  last_end,  last_y;

    // We delete first and last line
    if (textbox->highlight_start_pos.y + textbox->view_line_start > textbox->highlight_end_pos.y + textbox->view_line_start)
    {
        first_line = FV_GetElementFromArray(textbox->textbox_lines, (i32)textbox->highlight_start_pos.y + textbox->view_line_start);
        last_line  = FV_GetElementFromArray(textbox->textbox_lines, (i32)textbox->highlight_end_pos.y + textbox->view_line_start);

        first_len   = strlen(first_line);
        first_start = (i32)textbox->highlight_start_pos.x + 1;
        first_end   = first_len;
        first_y     = (i32)textbox->highlight_start_pos.y + textbox->view_line_start;

        last_len   = strlen(last_line);
        last_start = 0;
        last_end   = (i32)textbox->highlight_end_pos.x;
        last_y     = (i32)textbox->highlight_end_pos.y + textbox->view_line_start;
    }
    else
    {
        // TODO
    }

    i32 first_delete_count = first_end - first_start + 1;
    memmove(&first_line[first_start], &first_line[first_end + 1], first_len - first_end);
    first_len -= first_delete_count;
    first_line[first_len] = '\0';

    i32 last_delete_count = last_end - last_start + 1;
    memmove(&last_line[last_start], &last_line[last_end + 1], last_len - last_end);
    last_len -= last_delete_count;
    last_line[last_len] = '\0';

    // After that we delete full lines between
    i32 lines_deleted = 0;
    if (highlighted_lines >= 2)
    {
        for (i32 i = 0; i < highlighted_lines - 1; i++)
        {
            FV_DeleteElementFromArray(textbox->textbox_lines, (i + 1) + start_pos_y);
            lines_deleted++;
        }
    }

    if (first_end == strlen(first_line))
        FV_DeleteElementFromArray(textbox->textbox_lines, first_y);
    if (last_end == strlen(last_line))
        FV_DeleteElementFromArray(textbox->textbox_lines, last_y - lines_deleted);

    FV_ComponentTextBoxDisableHighlight(component, app);
}

void
FV_ComponentTextBoxBackspaceHighlight(fv_component_t* component, fv_app_t* app, SDL_Event event)
{
    fv_component_textbox_t* textbox = component->component_additional_data;

    // Are we dealing with multiline selection?
    if (textbox->highlight_multiply_lines)
    {
        FV_ComponentTextBoxBackspaceMultilineHighlight(component, app, textbox);
        return;
    }

    i32 lines_letters  = floor(log10(textbox->textbox_lines->length)) + 1;
    i32 left_padding   = round((lines_letters * textbox->font_size) * 1.3);
    i32 text_pos_start = textbox->pos.x + 12 + left_padding;

    // Uff we are not 
    char* current_line = FV_GetElementFromArray(textbox->textbox_lines, (i32)textbox->cursor.y);

    f32 start = (textbox->highlight_pos.x - text_pos_start) / glyph_size_;
    f32 end   = ((textbox->highlight_pos.x + textbox->highlight_size.x) - text_pos_start) / glyph_size_;
    i32 len   = strlen(current_line);

    if (start < 0 || end >= len || start > end) 
    {
        // invalid values
        FV_ERROR_NO_EXIT("invalid indices for deletion.", 0);
        return;
    }

    i32 delete_count = end - start + 1;
    memmove(&current_line[(i32)start], &current_line[(i32)end + 1], len - (i32)end);
    len -= delete_count;
    current_line[len] = '\0';

    FV_ComponentTextBoxDisableHighlight(component, app);
}

void
FV_ComponentTextBoxBackspaceKey(fv_component_t* component, fv_app_t* app, SDL_Event event)
{
    fv_component_textbox_t* textbox = component->component_additional_data;

    if (textbox->highlight)
    {
        FV_ComponentTextBoxBackspaceHighlight(component, app, event);
        return;
    }

    if (textbox->cursor.x == 0)
    {
        if (textbox->cursor.y == 0)
            return;

        char* line_return    = FV_GetElementFromArray(textbox->textbox_lines, textbox->cursor.y - 1);
        char* line_backspace = FV_GetElementFromArray(textbox->textbox_lines, textbox->cursor.y);

        size_t line_backspace_size = strlen(line_backspace);
        size_t line_return_size    = strlen(line_return);

        char* new_line_return = calloc((line_return_size + line_backspace_size), sizeof(char));
        strcpy(new_line_return, line_return);

        for (size_t i = line_return_size; i < (line_return_size + line_backspace_size); i++)
            new_line_return[i] = line_backspace[i - line_return_size];

        FV_DeleteElementFromArray(textbox->textbox_lines, textbox->cursor.y);
        FV_DeleteElementFromArray(textbox->textbox_lines, textbox->cursor.y - 1);
        FV_InsertElementInArray(textbox->textbox_lines, textbox->cursor.y - 1, new_line_return);
        FV_ComponentTextBoxDecrementCursorY(component, app);
        textbox->cursor.x = (float)strlen(new_line_return);

        return;
    }

    char* current_line    = FV_GetElementFromArray(textbox->textbox_lines, textbox->cursor.y);
    char* cursor_position = &current_line[(i32)textbox->cursor.x];

    // Shift the characters to the left
    memmove(cursor_position - 1, cursor_position, strlen(cursor_position) + 1);

    textbox->cursor.x--;
}

void 
FV_ComponentTextBoxAddChar(fv_component_textbox_t* textbox, char* current_line, char append_char)
{
    size_t current_line_length = strlen(current_line);

    strcpy(rest_of_line, current_line);
    free(current_line);
    for (size_t i = current_line_length + 1; i > textbox->cursor.x; --i) {
        rest_of_line[i] = rest_of_line[i - 1];
    }

    rest_of_line[(i32)textbox->cursor.x ] = append_char;
    rest_of_line[current_line_length + 1] = '\0';
    FV_DeleteElementFromArray(textbox->textbox_lines, textbox->cursor.y);
    FV_InsertElementInArray(textbox->textbox_lines, textbox->cursor.y, FV_DuplicateString(rest_of_line));
    memset(rest_of_line, 0, MAX_LINE_LENGTH);
    textbox->cursor.x++;
}

void
FV_ComponentTextBoxTextInput(fv_component_t* component, fv_app_t* app, SDL_Event event)
{
    fv_component_textbox_t* textbox = component->component_additional_data;

    char* current_line = FV_GetElementFromArray(textbox->textbox_lines, textbox->cursor.y);
    char append_char = event.text.text[0];

    FV_ComponentTextBoxAddChar(textbox, current_line, append_char);
}

void
FV_ComponentTextBoxTabKey(fv_component_t* component, fv_app_t* app, SDL_Event event)
{
    fv_component_textbox_t* textbox = component->component_additional_data;

    char* current_line = FV_GetElementFromArray(textbox->textbox_lines, textbox->cursor.y);
    char append_char   = '\t';

    FV_ComponentTextBoxAddChar(textbox, current_line, append_char);
}

void
FV_ComponentTextBoxEnterKey(fv_component_t* component, fv_app_t* app, SDL_Event event)
{
    fv_component_textbox_t* textbox = component->component_additional_data;

    if (textbox->cursor.x == 0)
    {
        FV_InsertElementInArray(textbox->textbox_lines, textbox->cursor.y, calloc(1, sizeof(char)));
        textbox->cursor.x = 0;
        FV_ComponentTextBoxIncrementCursorY(component, app);
        return;
    }
    
    char* line         = FV_GetElementFromArray(textbox->textbox_lines, textbox->cursor.y);
    size_t line_length = strlen(line);
    if (textbox->cursor.x != line_length)
    {
        /* This code is so messy but basically what it does it 
         * delete the rest of line and make it appear in new line */
        for (size_t i = textbox->cursor.x; i < line_length; i++)
            rest_of_line[i - (size_t)textbox->cursor.x] = line[i];
        rest_of_line[line_length] = '\0';
        line = realloc(line, (textbox->cursor.x + 1) * sizeof(char));
        line[(i32)textbox->cursor.x] = '\0';
        FV_InsertElementInArray(textbox->textbox_lines, textbox->cursor.y + 1, FV_DuplicateString(rest_of_line));
        memset(rest_of_line, 0, MAX_LINE_LENGTH);
    }
    else
        FV_InsertElementInArray(textbox->textbox_lines, textbox->cursor.y + 1, calloc(1, sizeof(char)));

    textbox->cursor.x = 0;
    FV_ComponentTextBoxIncrementCursorY(component, app);
}

fv_vector_t
FV_ComponentTextBoxDetermiteCursorInText(fv_component_t* component, i32 _mouse_x, i32 _mouse_y)
{
    fv_component_textbox_t* textbox = component->component_additional_data;

    fv_vector_t new_cursor = FV_NewVector(0, 0);

    i32 lines_letters  = floor(log10(textbox->textbox_lines->length)) + 1;
    i32 left_padding   = round((lines_letters * textbox->font_size) * 1.3);
    i32 text_pos_start = textbox->pos.x + 12 + left_padding;

    i32 mouse_x = _mouse_x - text_pos_start;
    i32 mouse_y = _mouse_y;

    /* Determite Y cursor position */
    f32 determite_y = mouse_y / (textbox->font_size + textbox->line_space);

    if ((i32)determite_y > textbox->textbox_lines->length)
        new_cursor.y = textbox->textbox_lines->length - 1;
    else
        new_cursor.y = (i32)determite_y + textbox->view_line_start;

    /* Determite X cursor position */
    size_t current_line_len = strlen(FV_GetElementFromArray(textbox->textbox_lines, textbox->cursor.y));
    char*  current_line     = FV_GetElementFromArray(textbox->textbox_lines, textbox->cursor.y);
    i32    x_cord = 0;
    i32    i      = 0;

    if (mouse_x < glyph_size_)
        i = -1;
    else
    {
        for (i = 0; i < current_line_len; i++)
        {
            char current_line_char = current_line[i];
            i32  adv = 0;
            TTF_GlyphMetrics32(textbox->font->font, current_line_char, NULL, NULL, NULL, NULL, &adv);
            adv         += 1;
            glyph_size_ =  adv;
            x_cord      += adv;
            if (x_cord <= mouse_x && (x_cord + adv) >= mouse_x)
                break;
            else
                continue;
        }
    }
        
    new_cursor.x = i + 1;
    return new_cursor;
}

void
FV_ComponentTextButtonMotion(fv_component_t* component, fv_app_t* app, SDL_Event event)
{
    SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM));
    fv_component_textbox_t* textbox = component->component_additional_data;

    if (textbox->mouse_button_state == true)
    {
        i32 mouse_x = event.motion.x;
        i32 mouse_y = event.motion.y;

        i32 lines_letters  = floor(log10(textbox->textbox_lines->length)) + 1;
        i32 left_padding   = round((lines_letters * textbox->font_size) * 1.3);
        i32 text_pos_start = textbox->pos.x + 12 + left_padding;

        fv_vector_t highlight_cursor = FV_ComponentTextBoxDetermiteCursorInText(component, mouse_x, mouse_y);
        FV_SUCCESS("start_cursor: .x=%f, .y=%f end_cursor: .x=%f, .y=%f, view_line_start: %d",
            textbox->cursor.x, textbox->cursor.y, highlight_cursor.x, highlight_cursor.y, textbox->view_line_start);
        highlight_cursor.y -= textbox->view_line_start;

        if (highlight_cursor.y >= textbox->textbox_lines->length)
            highlight_cursor.y = textbox->textbox_lines->length - 1;

        if (highlight_cursor.x > strlen(FV_GetElementFromArray(textbox->textbox_lines, highlight_cursor.y + textbox->view_line_start)))
            highlight_cursor.x = strlen(FV_GetElementFromArray(textbox->textbox_lines, highlight_cursor.y + textbox->view_line_start));

        if ((textbox->cursor.y - textbox->view_line_start) == highlight_cursor.y)
        {
            if (textbox->cursor.x > highlight_cursor.x)
            {
                // highlight_cursor.x is lower
                textbox->highlight_pos  = FV_NewVector(text_pos_start + (highlight_cursor.x * glyph_size_), 
                                                          textbox->pos.y + (highlight_cursor.y * (textbox->font_size + textbox->line_space)));
                textbox->highlight_size = FV_NewVector((textbox->cursor.x - highlight_cursor.x) * glyph_size_, textbox->font_size + textbox->line_space);
            }
            else
            {
                // highlight_cursor.x is bigger
                textbox->highlight_pos  = FV_NewVector(text_pos_start + (textbox->cursor.x * glyph_size_), 
                                                          textbox->pos.y + ((textbox->cursor.y - textbox->view_line_start) * (textbox->font_size + textbox->line_space)));
                textbox->highlight_size = FV_NewVector((highlight_cursor.x - textbox->cursor.x) * glyph_size_, textbox->font_size + textbox->line_space);
            }
            textbox->highlight_multiply_lines  = false;
            textbox->highlight = true;
        }
        else
        {
            textbox->highlight_multiply_lines = true;
            textbox->highlight_start_pos = textbox->cursor;
            textbox->highlight_end_pos = highlight_cursor;
            textbox->highlight = true;

            textbox->highlight_start_pos.y -= textbox->view_line_start;
        }
    }
}

void
FV_ComponentTextBoxSetCursorByMouse(fv_component_t* component, fv_app_t* app, SDL_Event event)
{
    fv_component_textbox_t* textbox = component->component_additional_data;

    i32 mouse_x = event.button.x;
    i32 mouse_y = event.button.y;

    fv_vector_t new_cursor = FV_ComponentTextBoxDetermiteCursorInText(component, mouse_x, mouse_y);

    size_t cursor_line_length = strlen(FV_GetElementFromArray(textbox->textbox_lines, new_cursor.y));
    if (cursor_line_length <= new_cursor.x)
        new_cursor.x -= 1;

    FV_SUCCESS("new cursor position: .x=%d, .y=%d, mouse_x:%d", 
        (i32)new_cursor.x, (i32)new_cursor.y, mouse_x);
    textbox->cursor = new_cursor;
    textbox->mouse_button_state = true; 

    if ((textbox->highlight_size.x != 0 && textbox->highlight_size.y != 0) || (textbox->highlight_multiply_lines))
        FV_ComponentTextBoxDisableHighlight(component, app);
}

void
FV_ComponentTextBoxHandleCtrlMod(fv_component_t* component, fv_app_t* app, SDL_Event event)
{
    fv_component_textbox_t* textbox = component->component_additional_data;

    i32 lines_letters  = floor(log10(textbox->textbox_lines->length)) + 1;
    i32 left_padding   = round((lines_letters * textbox->font_size) * 1.3);
    i32 text_pos_start = textbox->pos.x + 12 + left_padding;

    if (event.key.keysym.sym == SDLK_a)
    {
        FV_ComponentTextBoxDisableHighlight(component, app);
        if ((textbox->textbox_lines->length - 1) == 0)
        {
            char* line = FV_GetElementFromArray(textbox->textbox_lines, 0);

            textbox->highlight = true;
            textbox->highlight_multiply_lines = false;
            textbox->highlight_pos  = FV_NewVector(text_pos_start, textbox->pos.y);
            textbox->highlight_size = FV_NewVector(strlen(line) * glyph_size_, textbox->font_size + textbox->line_space);
        }
        else
        {
            char* last_line = FV_GetElementFromArray(textbox->textbox_lines, textbox->textbox_lines->length - 1);
            textbox->highlight = true;
            textbox->highlight_multiply_lines = true;
            textbox->highlight_start_pos = FV_NewVector(0, 0);
            textbox->highlight_end_pos = FV_NewVector(strlen(last_line), textbox->textbox_lines->length - 1);
        }
    }
}

void
FV_ComponentTextBoxMouseWheelEvent(fv_component_t* component, fv_app_t* app, SDL_Event event)
{
    fv_component_textbox_t* textbox = component->component_additional_data;

    if (event.wheel.y < 0)
        for (int i = 0; i < abs(event.wheel.y); i++)
            FV_ComponentTextBoxIncrementViewLine(component, app);
    else if (event.wheel.y > 0)
        for (int i = 0; i < abs(event.wheel.y); i++)
            FV_ComponentTextBoxDecrementViewLine(component, app);
}

int 
FV_ComponentTextBoxEventFunction(fv_component_t* component, fv_app_t* app, SDL_Event event)
{
    fv_component_textbox_t* textbox = component->component_additional_data;

    i32 lines_letters = floor(log10(textbox->textbox_lines->length)) + 1;
    i32 left_padding = round((lines_letters * textbox->font_size) * 1.3);
    bool mouse_collision = event.type == SDL_MOUSEBUTTONDOWN 
                                ? FV_CollisionBoxVector(FV_NewVector(textbox->pos.x + 12 + left_padding, textbox->pos.y), textbox->size, 
                                                        FV_NewVector(event.button.x, event.button.y), 
                                                        FV_NewVector(1, 1))
                                : false;

    if (event.type == SDL_MOUSEWHEEL)
        FV_ComponentTextBoxMouseWheelEvent(component, app, event);

    if (event.type == SDL_MOUSEMOTION)
    {
        bool motion_collision = FV_CollisionBoxVector(FV_NewVector(textbox->pos.x + 12 + left_padding, textbox->pos.y), textbox->size, 
                                                        FV_NewVector(event.motion.x, event.motion.y), 
                                                        FV_NewVector(1, 1));
        if (event.type == SDL_MOUSEMOTION && motion_collision)
            FV_ComponentTextButtonMotion(component, app, event);
        else if (event.type == SDL_MOUSEMOTION && !motion_collision)
            SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));
    }

    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
    {
        textbox->size.x = event.window.data1;
        textbox->size.y = event.window.data2;
    }

    if (event.type == SDL_KEYDOWN && textbox->disable_writting == false && textbox->focus)
    {   
        if (event.key.keysym.mod & FV_CONFIG_CONTROL_MOD)
        {
            FV_ComponentTextBoxHandleCtrlMod(component, app, event);
            return 0;
        }

        if (event.key.keysym.sym == SDLK_UP)
        {    
            if (textbox->cursor.y != 0)
                FV_ComponentTextBoxDecrementCursorY(component, app);
        }
        else if (event.key.keysym.sym == SDLK_RIGHT)
        {
            char* curr_line = FV_GetElementFromArray(textbox->textbox_lines, textbox->cursor.y);

            if (textbox->cursor.x != strlen(curr_line))
                textbox->cursor.x++;
            else
                if (textbox->cursor.y != textbox->textbox_lines->length)
                {
                    textbox->cursor.x = 0;
                    FV_ComponentTextBoxIncrementCursorY(component, app);
                }
        }
        else if (event.key.keysym.sym == SDLK_LEFT)
        {
            if (textbox->cursor.x != 0)
                textbox->cursor.x--;   
            else
                if (textbox->cursor.y != 0)
                {
                    textbox->cursor.x = strlen(FV_GetElementFromArray(textbox->textbox_lines, textbox->cursor.y - 1));
                    FV_ComponentTextBoxDecrementCursorY(component, app);
                }         
        }
        else if (event.key.keysym.sym == SDLK_DOWN)
        {
            if (textbox->cursor.y != (textbox->textbox_lines->length - 1))
                FV_ComponentTextBoxIncrementCursorY(component, app);       

            char* moved_line = FV_GetElementFromArray(textbox->textbox_lines, textbox->cursor.y);
            size_t moved_line_len = strlen(moved_line);
            if (moved_line_len < textbox->cursor.x)
                textbox->cursor.x = moved_line_len == 0 ? 0 : moved_line_len;
        } 
        else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER)
            FV_ComponentTextBoxEnterKey(component, app, event);
        else if (event.key.keysym.sym == SDLK_BACKSPACE)
            FV_ComponentTextBoxBackspaceKey(component, app, event);
        else if (event.key.keysym.sym == SDLK_TAB)
            FV_ComponentTextBoxTabKey(component, app, event);
    }

    if (event.type == SDL_TEXTINPUT)
        FV_ComponentTextBoxTextInput(component, app, event);

    if (event.type == SDL_MOUSEBUTTONDOWN && mouse_collision && !textbox->focus)
    {
        FV_SUCCESS("Focus on textbox [id=%d]", component->component_id);
        textbox->focus = true;
        SDL_StartTextInput();
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN && mouse_collision && textbox->focus)
        FV_ComponentTextBoxSetCursorByMouse(component, app, event);
    else if (event.type == SDL_MOUSEBUTTONDOWN && !mouse_collision)
    {
        textbox->focus = false;
        SDL_StopTextInput();
    }

    if (event.type == SDL_MOUSEBUTTONUP)
        textbox->mouse_button_state = false;

    return 0;
}   

int 
FV_ComponentTextBoxRunFunction(fv_component_t* component, fv_app_t* app)
{
    fv_component_textbox_t* textbox = component->component_additional_data;
    FV_SetFontSize(app->font_manager, textbox->font, textbox->font_size);
    return 0;
}   
