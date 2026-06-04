#pragma once
#include <SGL/SGL.h>
#include <SGL/Maths/Vec2.h>
#include <SGL/Graphics/GraphicsDevice.h>
#include <SGL/Graphics/Texture.h>

SGL_BEGIN

typedef struct sgl_Texture2D sgl_sealed
{
    sgl_Texture base;
} sgl_Texture2D;

SGL_API extern sgl_Texture* sgl_Texture2D_New_File(sgl_GraphicsDevice* device, const char* path);
SGL_API extern sgl_Texture* sgl_Texture2D_New_Source(sgl_GraphicsDevice* device, void* data, size_t dataSize);

SGL_END