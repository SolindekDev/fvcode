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

fv_array_t* 
FV_StringSplitByNewline(char* s)
{
    fv_array_t* split_arr = FV_CreateArray(sizeof(char*));
    size_t s_len          = strlen(s);

    size_t line_index     = 0;
    char* line            = malloc(2 * sizeof(char));

    for (int i = 0; i < s_len; i++)
    {
        char s_curr = s[i];

        switch (s_curr)
        {
            case '\n':
                FV_AppendElementToArray(split_arr, line);
                line_index = 0;
                line = malloc(2 * sizeof(char));
                break;
            default:
                line[line_index + 0] = s_curr;
                line[line_index + 1] = '\0'  ;
                line = realloc(line, (line_index + 2) * sizeof(char));
                line_index++;
                break;
        }
    }

    FV_ARRAY_FOR(split_arr)
        printf("[i=%d]=%s\n", i, (char*)FV_GetElementFromArray(split_arr, i));
    return split_arr;
}
