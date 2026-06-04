#pragma once
#include <SGL/Graphics/Texture.h>

SGL_BEGIN

typedef struct sgl_Texture2DArray sgl_sealed
{
    sgl_Texture base;
    sgl_Vec2i frameSize;
    uint32 textureCount;
} sgl_Texture2DArray;

SGL_API extern sgl_Texture* sgl_Texture2DArray_New_File(sgl_GraphicsDevice* device, const char* path, sgl_Vec2i frameSize);
SGL_API extern sgl_Texture* sgl_Texture2DArray_New_Source(sgl_GraphicsDevice* device, void* data, size_t dataSize, sgl_Vec2i frameSize);

SGL_END