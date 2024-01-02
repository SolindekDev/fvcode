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

#ifndef __FV_ALLOC_H
#define __FV_ALLOC_H

#include <fv/fv.h>

typedef struct __fv_alloc_pointer_t {
    void* ptr;
    u8    free;
} fv_alloc_pointer_t;

void FV_AllocInit ();
void FV_UnallocAll();

void*  FV_Calloc (size_t count, size_t size);
void*  FV_Malloc (size_t size);
void*  FV_Realloc(void *ptr, size_t size);

void   FV_Free(void *ptr);
size_t FV_GetAllocatedPointersLength();

fv_alloc_pointer_t* FV_FindAllocatedPointerByAddress(void* address);
fv_alloc_pointer_t* FV_CreateAllocatedPointer(void* ptr, u8 free);

#endif /* __FV_ALLOC_H */