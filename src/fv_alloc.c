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
void** fv_allocated_pointers;

void 
FV_AllocInit()
{
    if (fv_allocated_pointers != NULL)
        return;
    fv_allocated_pointers = calloc(1, sizeof(void*));
    return;
}

void 
FV_UnallocAll()
{

}

void* 
FV_Calloc(size_t count, size_t size)
{

}

void* 
FV_Malloc(size_t size)
{

}

void* 
FV_Realloc(void *ptr, size_t size)
{

}

void  
FV_Free(void *ptr)
{

}