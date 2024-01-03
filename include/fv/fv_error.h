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

#ifndef __FV_ERROR_H
#define __FV_ERROR_H

#include <fv/fv.h>

#define FV_ERROR_NO_EXIT(err) (FV_ErrorNoExit(__FILE__, __LINE__, __func__, err))
#define FV_ERROR(err)         (FV_Error(__FILE__, __LINE__, __func__, err))

__FV_NO_RETURN__ void FV_Error(const char* file, i32 line, 
                               const char* func, const char* error);

void FV_ErrorNoExit(const char* file, i32 line, 
                    const char* func, const char* error);
                    
#endif /* __FV_ERROR_H */