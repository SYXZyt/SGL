#pragma once
#include <SGL/SGL.h>

SGL_BEGIN

/// @brief Queue a VAO for deletion
/// @param vao The VAO to delete
extern void sgl_OpenGLThreadSync_DeleteVAO(gluint vao);

/// @brief Queue a VBO for deletion
/// @param vbo The VBO to delete
extern void sgl_OpenGLThreadSync_DeleteVBO(gluint vbo);

/// @brief Queue an EBO for deletion
/// @param ebo The EBO to delete
extern void sgl_OpenGLThreadSync_DeleteEBO(gluint ebo);

/// @brief Queue a shader for deletion
/// @param id The shader to delete
extern void sgl_OpenGLThreadSync_DeleteShader(gluint id);

/// @brief Delete all queued items and clear the queues
extern void sgl_OpenGLThreadSync_Update();

SGL_END