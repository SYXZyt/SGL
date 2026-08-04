#pragma once
#include <SGL/Graphics/Texture2DArray.h>
#include <glad/glad.h>

SGL_BEGIN

/// @brief OpenGL texture array
typedef struct sgl_GLTexture2DArray sgl_sealed
{
    sgl_Texture2DArray base;
    gluint texture;
} sgl_GLTexture2DArray;

/// @brief Create an OpenGL texture array
SGL_API extern sgl_GLTexture2DArray* sgl_GLTexture2DArray_Create(void* data, sgl_Vec2i atlasSize, sgl_Vec2i frameSize);

SGL_END