#pragma once
#include <SGL/Graphics/Texture2D.h>
#include <glad/glad.h>

SGL_BEGIN

/// @brief OpenGL shader
typedef struct sgl_GLTexture2D sgl_sealed
{
    sgl_Texture2D base;
    gluint texture;
    bool generateMipmaps;
} sgl_GLTexture2D;

/// @brief Create an OpenGL shader
SGL_API extern sgl_GLTexture2D* sgl_GLTexture2D_Create(void* data, sgl_Vec2i size, bool generateMipmaps);

SGL_END