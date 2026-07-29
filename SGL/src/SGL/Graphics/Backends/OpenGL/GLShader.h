#pragma once
#include <SGL/Graphics/Shader.h>

SGL_BEGIN

/// @brief OpenGL shader
typedef struct sgl_GLShader sgl_sealed
{
    sgl_Shader base;
    gluint shaderId;
} sgl_GLShader;

/// @brief Create an OpenGL shader
SGL_API extern sgl_GLShader* sgl_GLShader_Create();

SGL_END