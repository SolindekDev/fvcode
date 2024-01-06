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

#include <fv/fv_app.h>
#include <fv/fv_alloc.h>
#include <fv/fv_io.h>
#include <fv/fv_shaders.h>

fv_shaders_t* 
FV_ShadersInit()
{
    fv_shaders_t* shaders   = FV_Calloc(1, sizeof(fv_shaders_t));
    shaders->loaded_shaders = FV_CreateArray(sizeof(fv_shader_t));
    return shaders;
}

void
FV_CheckShaderCompileStatus(u32 shader_id, const char* file_path)
{
    /* Get compile status */
    i32 compile_status;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status);

    if (compile_status != true) 
    {
        /* Get shader error message length */
        GLint message_length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &message_length);

        /* Allocate memory for error message and then print it out 
         * as we should */
        char* error_msg = (char*)FV_Calloc(1, message_length);
        glGetShaderInfoLog(shader_id, message_length, NULL, error_msg);
        glDeleteShader(shader_id);

        FV_ERROR("Shader compilation error in shader file at \'%s\': \'%s\'", file_path, error_msg);
    }

    return;
}

void
FV_CheckShaderProgramLinkStatus(u32 program_id)
{
    /* Get compile status */
    i32 link_status;
    glGetShaderiv(program_id, GL_LINK_STATUS, &link_status);

    if (link_status != true) 
    {
        /* Get program link error message length */
        GLint message_length;
        glGetShaderiv(program_id, GL_INFO_LOG_LENGTH, &message_length);

        /* Allocate memory for error message and then print it out 
         * as we should */
        char* error_msg = (char*)FV_Calloc(1, message_length);
        glGetProgramInfoLog(program_id, message_length, NULL, error_msg);
        glDeleteShader(program_id);

        FV_ERROR("Shader program linking error: \'%s\'", error_msg);
    }

    return;
}

u32
FV_LoadShader(const char* file_path, GLenum shader_type)
{
    u32 shader_id = glCreateShader(GL_VERTEX_SHADER);
    printf("glCreateShader kurwo dzialaj\n");

    const char* shader_content = FV_ReadWholeFileContent((char*)file_path);
    if (shader_content == NULL)
        FV_ERROR("Couldn't load shader at \'%s\' properly", file_path);

    glShaderSource(shader_id, 1, &shader_content, NULL);
    glCompileShader(shader_id);

    FV_CheckShaderCompileStatus(shader_id, file_path);
    FV_Free((void*)shader_content);
    return shader_id;
}

u32
FV_CreateProgramAndAttachShaders(u32 vertex_shader, u32 frag_shader)
{
    u32 program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader);
    glAttachShader(program_id, frag_shader);
    glLinkProgram (program_id);
    return program_id;
}

fv_shader_t* 
FV_CreateShaderProgram(fv_shaders_t* shaders, const char* vertex_shader_path, const char* fragment_shader_path)
{
    u32 vertex_shader = FV_LoadShader(vertex_shader_path,   GL_VERTEX_SHADER);
    u32 frag_shader   = FV_LoadShader(fragment_shader_path, GL_VERTEX_SHADER);

    u32 program_id    = FV_CreateProgramAndAttachShaders(vertex_shader, frag_shader);
    FV_CheckShaderProgramLinkStatus(program_id);

    glDeleteShader(vertex_shader);
    glDeleteShader(frag_shader);

    fv_shader_t* shader = (fv_shader_t*)FV_Calloc(1, sizeof(fv_shader_t));
    shader->fragment_shader_path = vertex_shader_path;
    shader->vertex_shader_path   = fragment_shader_path;
    shader->program_id           = program_id;
    FV_AppendElementToArray(shaders->loaded_shaders, shader);
    return shader;
}

fv_shader_t*
FV_GetShaderProgram(fv_shaders_t* shaders, const char* vertex_shader_path, const char* fragment_shader_path)
{

}