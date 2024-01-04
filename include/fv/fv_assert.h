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

#ifndef __FV_ASSERT_H
#define __FV_ASSERT_H

#include <fv/fv.h>
#include <fv/fv_types.h>

#ifndef __FV_NO_RETURN__
#define __FV_NO_RETURN__ __attribute__((noreturn))
#endif /* __FV_NO_RETURN__ */

#define FV_ASSERT(e) {(e == true)                                            \
                            ? (e)                                            \
                            : FV_Assert(__FILE__, __LINE__, __func__, #e); } \

__FV_NO_RETURN__ void FV_Assert(const char* file, i32 line, 
                                const char* func, const char* e);

#endif /* __FV_ASSERT_H */