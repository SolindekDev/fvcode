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

#include <fv/fv_component_textbox.h>
#include <fv/fv_component_manager.h>
#include <fv/fv_component.h>

#include <fv/fv_font_manager.h>
#include <fv/fv_font_draw.h>

#include <fv/fv_collisions.h>
#include <fv/fv_drawing.h>
#include <fv/fv_alloc.h>
#include <fv/fv_msg.h>

fv_component_t*
FV_CreateComponentTextBox(fv_vector_t pos, fv_vector_t size, fv_color_t bg, fv_color_t fg, 
                          char* textbox_value, fv_font_t* font, i32 font_size, fv_color_t border_color)
{   
    fv_component_t* textbox_component   = FV_CreateComponent("textbox", COMPONENT_TEXTBOX);
    textbox_component->component_render = FV_ComponentTextBoxRenderFunction;
    textbox_component->component_event  = FV_ComponentTextBoxEventFunction;
    textbox_component->component_run    = FV_ComponentTextBoxRunFunction;

    fv_component_textbox_t* textbox = FV_Calloc(1, sizeof(fv_component_textbox_t));
    textbox->size             = size;
    textbox->pos              = pos;
    textbox->bg               = bg;
    textbox->fg               = fg;
    textbox->border_color     = border_color;
    textbox->textbox_value    = textbox_value;
    textbox->font             = font;
    textbox->font_size        = font_size;
    textbox->disable_writting = false;

    textbox_component->component_additional_data = textbox;
    return textbox_component;
}

void
FV_ComponentTextBoxRenderCursor(fv_component_t* component, fv_app_t* app)
{
    fv_component_textbox_t* textbox = component->component_additional_data;

    int cursor_x = 0;
    int cursor_y = 0;

    if (textbox->cursor.y == 0)
    {
        for (int i = 0; i < textbox->cursor.x; i++)
        {   
            int temp_cursor_x = 0;
            TTF_GlyphMetrics32(textbox->font->font, textbox->textbox_value[i], NULL, NULL, NULL, &temp_cursor_x, NULL);
            cursor_x += temp_cursor_x;
        }

        cursor_x += (textbox->pos.x + 7);
        cursor_y =  (textbox->pos.y + 11); 
    }
    else
    {

    }

    FV_DrawFillRect(app, FV_NewVector(cursor_x, cursor_y), FV_NewVector(2, textbox->font_size), FV_NewColorRGB(255, 255, 255, 255));
}

int 
FV_ComponentTextBoxRenderFunction(fv_component_t* component, fv_app_t* app)
{
    fv_component_textbox_t* textbox = component->component_additional_data;

    FV_DrawFillRect(app, textbox->pos, textbox->size, textbox->bg);
    FV_DrawRect(app, textbox->pos, textbox->size, textbox->border_color);
    FV_RenderFont(app, textbox->font, textbox->font_size, textbox->size.x + textbox->pos.x, textbox->fg, FV_NewVector(textbox->pos.x + 7, textbox->pos.x + 7), textbox->textbox_value);
    
    FV_RenderFontFormat(app, textbox->font, 14, 1000, FV_NewColorRGB(255, 255, 255, 255), FV_NewVector(20, 500), 
        "Cursor: x=%d, y=%d", 
            textbox->cursor.x, textbox->cursor.y);
    FV_SUCCESS("Cursor: x=%d, y=%d", 
            textbox->cursor.x, textbox->cursor.y);

    if (textbox->focus == true)
        FV_ComponentTextBoxRenderCursor(component, app);

    return 0;
}

int 
FV_ComponentTextBoxEventFunction(fv_component_t* component, fv_app_t* app, SDL_Event event)
{
    fv_component_textbox_t* textbox = component->component_additional_data;

    bool mouse_collision = event.type == SDL_MOUSEBUTTONDOWN 
                                ? FV_CollisionBoxVector(textbox->pos, textbox->size, FV_NewVector(event.button.x, event.button.y), FV_NewVector(2, 2))
                                : false;

    if (event.type == SDL_KEYDOWN && textbox->disable_writting == false && textbox->focus)
    {   
        if (event.key.keysym.sym == SDLK_UP)
        {    
            if (textbox->cursor.y != 0)
                textbox->cursor.y--;
        }
        else if (event.key.keysym.sym == SDLK_RIGHT)
            textbox->cursor.x++;
        else if (event.key.keysym.sym == SDLK_LEFT)
            textbox->cursor.x--;
        else if (event.key.keysym.sym == SDLK_DOWN)
            textbox->cursor.y++;
    }
    if (event.type == SDL_MOUSEBUTTONDOWN && mouse_collision)
    {
        FV_SUCCESS("Focus on textbox [id=%d]", component->component_id);
        textbox->focus = true;
    }

    return 0;
}   

int 
FV_ComponentTextBoxRunFunction(fv_component_t* component, fv_app_t* app)
{
    return 0;
}   
