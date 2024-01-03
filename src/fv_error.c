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

#include <fv/fv_error.h>
#include <fv/fv_alloc.h>
#include <fv/fv_app.h>

#define FV_ERROR_MESSAGE                                      \
     "FVCode Error: %s:%d in %s: %s\nExiting with 1 code.\n"  \

#define FV_ERROR_NO_EXIT_MESSAGE         \
     "FVCode Error: %s:%d in %s: %s\n"   \

__FV_NO_RETURN__ void 
FV_Error(const char* file, i32 line, 
         const char* func, const char* error)
{
    printf(FV_ERROR_MESSAGE, file, line, func, error);
    FV_UnallocAll();
    glfwTerminate();
    exit(1);
}


void FV_ErrorNoExit(const char* file, i32 line, 
                    const char* func, const char* error)
{
    printf(FV_ERROR_NO_EXIT_MESSAGE, file, line, func, error);
}