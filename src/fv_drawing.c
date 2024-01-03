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

#include <fv/fv_vector.h>
#include <fv/fv_render.h>
#include <fv/fv_color.h>
#include <fv/fv_app.h>

/* app - pointer to app strucutre
 * start - start position of line
 * end - end position of line
 * line_color - line color 
 * border_tickness - if 0 then function will fail*/
void
FV_DrawRenderLine(fv_app_t* app, fv_vector_t start, fv_vector_t end,
                  fv_color_t line_color, i32 line_thickness)
{
    glLineWidth(line_thickness);
    glBegin(GL_LINES);
        glColor4f(FV_CONVERT_COLOR_TO_OPENGL(line_color));
        glVertex2f(start.x, start.y);
        glVertex2f(end.x,   end.y);
    glEnd();
}

/* app - pointer to app strucutre
 * pos - position of quad
 * size - size of quad 
 * fill_color - fill color 
 * border_color - border color
 * border_tickness - if 0 then no border */
void 
FV_DrawRenderQuad(fv_app_t* app, fv_vector_t pos, fv_vector_t size,
                  fv_color_t fill_color, fv_color_t border_color, i32 border_thickness)
{
    glColor4f(FV_CONVERT_COLOR_TO_OPENGL(fill_color));
    glBegin(GL_QUADS);
        glVertex2f(pos.x, pos.y);
        glVertex2f(pos.x + size.x, pos.y);
        glVertex2f(pos.x + size.x, pos.y + size.y);
        glVertex2f(pos.x, pos.y + size.y);
    glEnd();

    if (border_thickness != 0)
    {
        glColor4f(FV_CONVERT_COLOR_TO_OPENGL(border_color));
        glLineWidth(border_thickness);
        glBegin(GL_LINE_LOOP);
            glVertex2f(pos.x, pos.y);
            glVertex2f(pos.x + size.x, pos.y);
            glVertex2f(pos.x + size.x, pos.y + size.y);
            glVertex2f(pos.x, pos.y + size.y);
        glEnd();
    }
}