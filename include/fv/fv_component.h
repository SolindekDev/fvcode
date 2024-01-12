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

#ifndef __FV_COMPONENT_H
#define __FV_COMPONENT_H

#include <fv/fv.h>

typedef enum __fv_component_kind_t {
    COMPONENT_LABLE
} fv_component_kind_t;

typedef struct __fv_component_t {
    const char*         component_name;
    fv_component_kind_t component_kind;
    /* functions */
} fv_component_t;

#endif /* __FV_COMPONENT_H */