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

#ifndef __FV_COLLISIONS_H
#define __FV_COLLISIONS_H

#include <fv/fv.h>
#include <fv/fv_vector.h>

bool FV_CollisionBox(i32 x1, i32 y1, i32 w1, i32 h1, 
                     i32 x2, i32 y2, i32 w2, i32 h2);

bool FV_CollisionBoxVector(fv_vector_t pos1, fv_vector_t size1, 
                           fv_vector_t pos2, fv_vector_t size2);

#endif /* __FV_COLLISIONS_H */