#pragma once
#include <SGL/Graphics/Shader.h>

SGL_BEGIN

typedef struct sgl_GLShader sgl_sealed
{
    sgl_Shader base;
    gluint shaderId;
} sgl_GLShader;

SGL_API extern sgl_GLShader* sgl_GLShader_Create();

SGL_END