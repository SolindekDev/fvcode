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



size_t
FV_GetFileSize(FILE* f)
{
    fseek(f, 0, SEEK_END);
    size_t res = ftell(f);
    fseek(f, 0, SEEK_SET);
    return res;
}

const char* 
FV_ReadWholeFileContent(char* filename)
{
    FILE* f = fopen(filename, "rb");

    /* File doesn't exists */
    if (f == NULL) 
        return NULL;

    size_t file_size = FV_GetFileSize(f);
    char* buffer     = calloc(file_size + 1, sizeof(char));
    FV_NO_NULL(buffer);

    /* Return value of fread need to be equal to
     * file_size because that's how it works lol */
    assert(
        fread(buffer, 1, file_size, f) == file_size);

    buffer[file_size] = '\0';
    fclose(f);
    return buffer;
}