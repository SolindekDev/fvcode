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

#include <fv/fv_msg.h>

fv_component_t*
FV_CreateComponentTextBox(fv_vector_t pos, fv_vector_t size, fv_color_t bg, fv_color_t fg, char* textbox_value, 
                          fv_font_t* font, i32 font_size, fv_color_t border_color, float line_space)
{   
    fv_component_t* textbox_component   = FV_CreateComponent("textbox", COMPONENT_TEXTBOX);
    textbox_component->component_render = FV_ComponentTextBoxRenderFunction;
    textbox_component->component_event  = FV_ComponentTextBoxEventFunction;
    textbox_component->component_run    = FV_ComponentTextBoxRunFunction;
    textbox_component->component_resize = true;
    textbox_component->component_size   = size;

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

    textbox->textbox_value = calloc(strlen(textbox_value) + 1, sizeof(char));
    memcpy(textbox->textbox_value, textbox_value, strlen(textbox_value) + 1);

    textbox->textbox_lines = FV_StringSplitByNewline(textbox->textbox_value);

    textbox_component->component_additional_data = textbox;
    return textbox_component;
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
    i32 lines_letters = floor(log10(abs(textbox->textbox_lines->length))) + 1;
    i32 left_padding = round((lines_letters * textbox->font_size) * 1.3);
    fv_vector_t line_vector_pos = FV_NewVector(textbox->pos.x + 10, textbox->pos.y);

    FV_DrawFillRect(app, FV_NewVector(textbox->pos.x, textbox->pos.y), 
                    FV_NewVector(textbox->pos.x + left_padding, textbox->pos.y + component->component_size.y),
                    textbox->border_color);

    char line_num_buffer[64];
    FV_SetFontSize(app->font_manager, textbox->font, textbox->font_size);
    for (int i = 0; i < textbox->textbox_lines->length; i++)
    {
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
            .w = font_size.x,                             .h = font_size.y
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

    i32 lines_letters = floor(log10(abs(textbox->textbox_lines->length))) + 1;
    i32 left_padding = round((lines_letters * textbox->font_size) * 1.3);
    fv_vector_t character_vector_pos = FV_NewVector(textbox->pos.x + 12 + left_padding, textbox->pos.y);

    FV_ARRAY_FOR(textbox->textbox_lines)
    {
        FV_ComponentTextBoxRenderLine(component, app, i, &character_vector_pos, left_padding);
        character_vector_pos.y += textbox->font_size + textbox->line_space;
    }
}

void
FV_ComponentTextBoxRenderInfo(fv_component_t* component, fv_app_t* app)
{
    fv_component_textbox_t* textbox = component->component_additional_data;

    i32 lines_letters = floor(log10(abs(textbox->textbox_lines->length))) + 1;
    i32 left_padding = round((lines_letters * textbox->font_size) * 1.3);
    fv_vector_t pos = FV_NewVector(textbox->pos.x, component->component_size.y - (textbox->font_size));

    FV_DrawFillRect(app, pos, FV_NewVector(component->component_size.x - pos.x, (textbox->font_size)), 
                    textbox->border_color);
    FV_RenderFontFormat(app, textbox->font, textbox->font_size - 4, 1280, FV_NewColorRGB(180, 180, 180, 255),
                        FV_NewVector(pos.x + 10, pos.y), "Total lines: %d", textbox->textbox_lines->length);
    FV_SetFontSize(app->font_manager, textbox->font, textbox->font_size);
}

int 
FV_ComponentTextBoxRenderFunction(fv_component_t* component, fv_app_t* app)
{
    fv_component_textbox_t* textbox = component->component_additional_data;

    FV_DrawFillRect(app, textbox->pos, component->component_size, textbox->bg);
    FV_DrawRect(app, textbox->pos, component->component_size, textbox->border_color);
    // FV_RenderFont(app, textbox->font, textbox->font_size, component->component_size.x + textbox->pos.x, textbox->fg, FV_NewVector(textbox->pos.x + 7, textbox->pos.y), textbox->textbox_value);
    
    FV_ComponentTextBoxRenderText(component, app);
    FV_ComponentTextBoxRenderLineNumbers(component, app);
    FV_ComponentTextBoxRenderInfo(component, app);

    return 0;
}

int 
FV_ComponentTextBoxEventFunction(fv_component_t* component, fv_app_t* app, SDL_Event event)
{
    fv_component_textbox_t* textbox = component->component_additional_data;

    bool mouse_collision = event.type == SDL_MOUSEBUTTONDOWN 
                                ? FV_CollisionBoxVector(textbox->pos, component->component_size, FV_NewVector(event.button.x, event.button.y), FV_NewVector(2, 2))
                                : false;

    if (event.type == SDL_KEYDOWN && textbox->disable_writting == false && textbox->focus)
    {   
        if (event.key.keysym.sym == SDLK_UP)
        {    
            if (textbox->cursor.y != 0)
                textbox->cursor.y--;
        }
        else if (event.key.keysym.sym == SDLK_RIGHT)
        {
            char* curr_line = FV_GetElementFromArray(textbox->textbox_lines, textbox->cursor.y);

            if (textbox->cursor.x != strlen(curr_line))
                textbox->cursor.x++;
        }
        else if (event.key.keysym.sym == SDLK_LEFT)
        {
            if (textbox->cursor.x != 0)
                textbox->cursor.x--;            
        }
        else if (event.key.keysym.sym == SDLK_DOWN)
        {
            if (textbox->cursor.y != (textbox->textbox_lines->length - 1))
                textbox->cursor.y++;       

            char* moved_line = FV_GetElementFromArray(textbox->textbox_lines, textbox->cursor.y);
            size_t moved_line_len = strlen(moved_line);
            if (moved_line_len < textbox->cursor.x)
                textbox->cursor.x = moved_line_len == 0 ? 0 : moved_line_len;
        } 
        else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER)
        {
            textbox->cursor.y++;
            textbox->cursor.x = 0;
            FV_InsertElementInArray(textbox->textbox_lines, textbox->cursor.y, calloc(1, sizeof(char)));
        }
        else if (event.key.keysym.sym == SDLK_BACKSPACE)
        {
            
        }
    }

    if (event.type == SDL_TEXTINPUT)
    {
        // char* new_textbox_value = realloc(textbox->textbox_value, (strlen(textbox->textbox_value) + 2) * sizeof(char));
        // FV_NO_NULL(new_textbox_value);

        // textbox->textbox_value = new_textbox_value;
        // for (size_t i = strlen(new_textbox_value); i > textbox->cursor.x; i--)
        //     textbox->textbox_value[i] = textbox->textbox_value[i - 1];

        // textbox->textbox_value[(int)textbox->cursor.x] = event.text.text[0];
        // textbox->cursor.x++;
    }

    if (event.type == SDL_MOUSEBUTTONDOWN && mouse_collision)
    {
        FV_SUCCESS("Focus on textbox [id=%d]", component->component_id);
        textbox->focus = true;
        SDL_StartTextInput();
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN && !mouse_collision)
    {
        textbox->focus = false;
        SDL_StopTextInput();
    }

    return 0;
}   

int 
FV_ComponentTextBoxRunFunction(fv_component_t* component, fv_app_t* app)
{
    // fv_component_textbox_t* textbox = component->component_additional_data;
    // FV_SetFontSize(app->font_manager, textbox->font, textbox->font_size);
    return 0;
}   
