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

#include <fv/fv_string.h>
#include <fv/fv_array.h>

#include <fv/fv_app.h>

char*
FV_DuplicateString(char* str)
{
    char* dup_string = (char*)calloc(strlen(str) + 1, sizeof(char));

    FV_NO_NULL(dup_string);
    strcpy(dup_string, str);

    return dup_string;
}

fv_array_t* 
FV_StringSplitByNewline(char* s)
{
    fv_array_t* split_arr = FV_CreateArray(sizeof(char*));

    static char line_buffer[MAX_LINE_LENGTH];
    size_t s_len = strlen(s);

    i32 line_index = 0;
    i32 char_index = 0;

    for (i32 i = 0; i < s_len; i++)
    {
        char s_char = s[i];

        if (s_char == '\n') 
        {
            line_buffer[char_index] = '\0';

            char* line_copy = FV_DuplicateString(line_buffer);
            FV_NO_NULL(line_copy);

            FV_AppendElementToArray(split_arr, line_copy);
            line_index += 1;
            char_index =  0;
            memset(line_buffer, 0, MAX_LINE_LENGTH); 
        } 
        else 
        {
            if (char_index < (MAX_LINE_LENGTH - 1)) 
            {
                line_buffer[char_index] = s_char;
                char_index++;
            } 
            else
                FV_ERROR_NO_EXIT("%d line is too long (1024 * 16 limit), skipping excess characters",
                                    MAX_LINE_LENGTH);
        }
    }

    char* line_copy = FV_DuplicateString(line_buffer);
    FV_NO_NULL(line_copy);
    FV_AppendElementToArray(split_arr, line_copy);
    return split_arr;
}
