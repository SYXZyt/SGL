#pragma once
#include <SGL/Graphics/Texture2D.h>
#include <glad/glad.h>

SGL_BEGIN

typedef struct sgl_GLTexture2D sgl_sealed
{
    sgl_Texture2D base;
    gluint texture;
} sgl_GLTexture2D;

SGL_API extern sgl_GLTexture2D* sgl_GLTexture2D_Create(void* data, sgl_Vec2i size);

SGL_END