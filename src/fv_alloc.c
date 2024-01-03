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

#include <fv/fv_alloc.h>
#include <fv/fv_assert.h>

/* Global array that consists of every allocated 
 * pointer by calloc, malloc etc. in this app.
 * This helps track memory allocation. */
fv_alloc_pointer_t** fv_allocated_pointers;

fv_alloc_pointer_t*
FV_CreateAllocatedPointer(void* mem_ptr, u8 free)
{
    fv_alloc_pointer_t* ptr = calloc(1, sizeof(fv_alloc_pointer_t));

    ptr->free = free;
    ptr->ptr  = mem_ptr;

    return ptr;
}

void 
FV_AllocInit()
{
    if (fv_allocated_pointers != NULL)
        return;
    fv_allocated_pointers  = calloc(1, sizeof(fv_alloc_pointer_t*));
    *fv_allocated_pointers = NULL;
}

void 
FV_UnallocAll()
{
    fv_alloc_pointer_t** pointers = fv_allocated_pointers;
    while (*pointers != NULL)
    {
        fv_alloc_pointer_t* curr_pointer = *pointers;
        if (curr_pointer->free == 0)
        {
            free(curr_pointer->ptr); 
            free(curr_pointer);   
        }
        pointers++;
    }
}

size_t
FV_GetAllocatedPointersLength()
{
    size_t ptrs_len = 0;
    while (fv_allocated_pointers[ptrs_len] != NULL)
        ptrs_len++;
    return ptrs_len;
}

void
FV_AppendAllocatedPointer(fv_alloc_pointer_t* alloc_ptr)
{
    size_t pointers_len    = FV_GetAllocatedPointersLength();
    fv_allocated_pointers = realloc(fv_allocated_pointers, 
                                    (pointers_len + 2) * sizeof(fv_alloc_pointer_t*));

    fv_allocated_pointers[pointers_len]    = alloc_ptr;
    fv_allocated_pointers[pointers_len + 1] = NULL;
}

void* 
FV_Calloc(size_t count, size_t size)
{
    void* mem_ptr = calloc(count, size);
    FV_ASSERT(mem_ptr != NULL);

    // Create a new allocated pointer that will go into fv_allocated_pointers
    fv_alloc_pointer_t* alloc_ptr = FV_CreateAllocatedPointer(mem_ptr, 0);
    FV_AppendAllocatedPointer(alloc_ptr);

    return mem_ptr;
}

void* 
FV_Malloc(size_t size)
{
    void* mem_ptr = malloc(size);
    FV_ASSERT(mem_ptr != NULL);

    // Create a new allocated pointer that will go into fv_allocated_pointers
    fv_alloc_pointer_t* alloc_ptr = FV_CreateAllocatedPointer(mem_ptr, 0);
    FV_AppendAllocatedPointer(alloc_ptr);

    return mem_ptr;
}

fv_alloc_pointer_t*
FV_FindAllocatedPointerByAddress(void* address)
{
    while (*fv_allocated_pointers != NULL)
    {
        if ((*fv_allocated_pointers)->ptr == address)
            return *fv_allocated_pointers;
        fv_allocated_pointers++;
    }

    return NULL;
}

void* 
FV_Realloc(void* ptr, size_t size)
{
    FV_ASSERT(ptr != NULL);

    fv_alloc_pointer_t* alloc_ptr = FV_FindAllocatedPointerByAddress(ptr);
    FV_ASSERT(alloc_ptr != NULL);

    alloc_ptr->ptr = realloc(ptr, size);
    FV_ASSERT(alloc_ptr->ptr != NULL);
    return alloc_ptr->ptr;
}

void  
FV_Free(void* ptr)
{
    FV_ASSERT(ptr != NULL);

    fv_alloc_pointer_t* alloc_ptr = FV_FindAllocatedPointerByAddress(ptr);
    FV_ASSERT(alloc_ptr != NULL);

    alloc_ptr->free = 1;
    free(ptr);
}