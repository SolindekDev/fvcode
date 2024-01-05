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

#ifndef __FV_FV_H
#define __FV_FV_H

#if defined(__MACH__) || defined(__APPLE__)
# define GL_SILENCE_DEPRECATION
#endif /* __MACH__, __APPLE__ */

/* C Headers */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

/* FV Headers */
#include <fv/fv_assert.h>
#include <fv/fv_types.h>
#include <fv/fv_msg.h>

#define __FV_NO_RETURN__ __attribute__((noreturn))

#define FV_NO_NULL(ptr) ({(ptr == NULL)                                                 \
                            ? FV_Error(__FILE__, __LINE__, __func__, #ptr " is NULL.")  \
                            : ((void*)0); })
#define FV_TRY(i)       ({ (i == -1)                                                        \
                            ? FV_Error(__FILE__, __LINE__, __func__, "\'" #i "\' failed.")  \
                            : ((void*)0); })

#endif /* __FV_FV_H */