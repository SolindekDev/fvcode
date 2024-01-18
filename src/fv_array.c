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

#include <fv/fv_array.h> 
#include <fv/fv_assert.h>

#include <fv/fv_msg.h> 

#ifndef ARRAY_VALIDATE_NUMBER
# define ARRAY_VALIDATE_NUMBER(num) ({                                                 \
    if (num < 0)                                                                       \
        FV_Error(__FILE__, __LINE__, __func__,                                         \
                "ARRAY_VALIDATE_NUMBER() went wrong while creating/modifing array");   \
 })
#endif /* ARRAY_VALIDATE_NUMBER */

fv_array_t* 
FV_CreateArray(size_t size_of_element)
{
    ARRAY_VALIDATE_NUMBER(size_of_element);

    /* Initialize array fields with default 
     * values */
    fv_array_t* array = (fv_array_t*)calloc(1, sizeof(fv_array_t));
    FV_NO_NULL(array);

    array->size_of_element = size_of_element;
    array->length          = 0;
    array->ptr             = (void**)calloc(array->length + 1, sizeof(void*));

    return array;
}

fv_array_t* 
FV_DestroyArray(fv_array_t* array)
{
    if (array == NULL) return NULL;

    free(array->ptr);
    free(array);

    return NULL;
}

void* 
FV_GetElementFromArray(fv_array_t* array, size_t index)
{
    if (array == NULL || index >= array->length || index < 0)
        return NULL;

    return array->ptr[index];
}

i32 
FV_AppendElementToArray(fv_array_t* array, void* element)
{
    if (array == NULL || element == NULL)
        return -1;

    void** new_ptr = (void**)realloc(array->ptr, (array->length + 1) * sizeof(void*));
    FV_NO_NULL(new_ptr);

    array->ptr = new_ptr;
    array->ptr[array->length] = element;
    array->length++;

    return 0;
}

i32 
FV_DeleteElementFromArray(fv_array_t* array, size_t index)
{
    if (array == NULL || index >= array->length)
        return -1;

    size_t bytesToMove = (array->length - index - 1) * sizeof(void*);
    memmove(&(array->ptr[index]), &(array->ptr[index + 1]), bytesToMove);

    array->ptr[array->length - 1] = NULL;
    array->length--;

    void** new_ptr = (void**)realloc(array->ptr, array->length * sizeof(void*));

    if (new_ptr != NULL || array->length == 0)
        array->ptr = new_ptr;

    return 0;  // Success
}

i32 
FV_DeleteLastElementFromArray(fv_array_t* array)
{
    if (array == NULL || array->length == 0 || array->ptr == NULL) 
        return -1;

    size_t new_length = array->length - 1;
    void** new_ptr = realloc(array->ptr, new_length * sizeof(void*));
    FV_NO_NULL(new_ptr);
    
    if (array->size_of_element > 0) 
    {
        void* last_element = array->ptr[array->length - 1];
        free(last_element);
    }

    array->ptr = new_ptr;
    array->length = new_length;

    return 0; 
}

i32
FV_ResizeArray(fv_array_t* array, size_t n)
{
    if (array == NULL)
        return -1;

    if (n == array->length)
        return 0;

    void** new_ptr = (void**)realloc(array->ptr, n * sizeof(void*));
    FV_NO_NULL(new_ptr);
    array->ptr = new_ptr;

    if (n < array->length) 
    {
        for (size_t i = n; i < array->length; i++) 
            if (array->size_of_element > 0) 
                free(array->ptr[i]);
    } 
    else 
    {
        for (size_t i = array->length; i < n; i++) 
            array->ptr[i] = NULL;
    }

    array->length = n;
    return 0;
}

i32 
FV_InsertElementInArray(fv_array_t* array, size_t index, void* element)
{
    if (array == NULL || element == NULL || index > array->length)
        return -1;

    void** new_ptr = (void**)realloc(array->ptr, (array->length + 1) * sizeof(void*));
    FV_NO_NULL(new_ptr);

    array->ptr = new_ptr;

    for (size_t i = array->length; i > index; i--)
        array->ptr[i] = array->ptr[i - 1];

    array->ptr[index] = element;
    array->length++;

    return 0;
}