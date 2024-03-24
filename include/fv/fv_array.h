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

#ifndef __FV_ARRAY_H
#define __FV_ARRAY_H

#include <fv/fv.h>

typedef struct __fv_array_t {
    size_t size_of_element;
    size_t length;
    void** ptr;
} fv_array_t;

#define FV_ARRAY_FOR(arr) for (i32 i = 0; i < arr->length; i++)
#define FV_ARRAY_IS_LAST_ELEMENT(arr, i) (arr->length == (i + 1))

fv_array_t* FV_CreateArray(size_t size_of_element);
fv_array_t* FV_DestroyArray(fv_array_t* array);

void* FV_GetElementFromArray(fv_array_t* array, size_t index);

i32 FV_AppendElementToArray(fv_array_t* array, void* element);
i32 FV_DeleteElementFromArray(fv_array_t* array, size_t index);
i32 FV_InsertElementInArray(fv_array_t* array, size_t index, void* element);
i32 FV_DeleteLastElementFromArray(fv_array_t* array);
i32 FV_ResizeArray(fv_array_t* array, size_t n);

#endif /* __FV_ARRAY_H */