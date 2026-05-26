#pragma once
#include <SGL/SGL.h>

SGL_BEGIN

extern void sgl_OpenGLThreadSync_DeleteVAO(gluint vao);
extern void sgl_OpenGLThreadSync_DeleteVBO(gluint vbo);
extern void sgl_OpenGLThreadSync_DeleteEBO(gluint ebo);
extern void sgl_OpenGLThreadSync_DeleteShader(gluint id);

extern void sgl_OpenGLThreadSync_Update();

SGL_END