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

#ifndef __FV_SHADERS_H
#define __FV_SHADERS_H

#include <fv/fv.h>
#include <fv/fv_app.h>
#include <fv/fv_array.h>

typedef struct __fv_shader_t {
    const char* fragment_shader_path;
    const char* vertex_shader_path;
    u32         program_id;
} fv_shader_t;

typedef struct __fv_shaders_t {
    fv_array_t* loaded_shaders;
} fv_shaders_t;
 
fv_shaders_t* FV_ShadersInit();

fv_shader_t* FV_CreateShaderProgram(fv_shaders_t* shaders, const char* vertex_shader_path, const char* fragment_shader_path);
fv_shader_t* FV_GetShaderProgram(fv_shaders_t* shaders, const char* vertex_shader_path, const char* fragment_shader_path);

u32 FV_LoadShader(const char* file_path, GLenum shader_type);

#endif /* __FV_SHADERS_H */