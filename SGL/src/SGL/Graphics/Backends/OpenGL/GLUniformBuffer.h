#pragma once
#include <SGL/Graphics/UniformBuffer.h>
#include <glad/glad.h>

SGL_BEGIN

typedef struct sgl_GLUniformBuffer sgl_sealed
{
    sgl_UniformBuffer base;
    gluint buffer;
} sgl_GLUniformBuffer;

SGL_API extern sgl_GLUniformBuffer* sgl_GLUniformBuffer_New(sgl_GraphicsDevice* gpu, size_t size);

SGL_END