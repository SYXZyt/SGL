#pragma once
#include <SGL/SGL.h>
#include <SGL/Maths/Vec2.h>
#include <SGL/Graphics/GraphicsDevice.h>

SGL_BEGIN

struct sgl_Texture2D;
typedef struct sgl_Texture2DVTable sgl_sealed
{
    void (*Destroy)(struct sgl_Texture2D* self);
    void (*Bind)(struct sgl_Texture2D* self, uint32 textureUnit);
} sgl_Texture2DVTable;

typedef struct sgl_Texture2D sgl_sealed
{
    const sgl_Texture2DVTable* vtable;
    sgl_GraphicsDevice* gpu;
    sgl_Vec2i size;
} sgl_Texture2D;

SGL_API extern sgl_Texture2D* sgl_Texture2D_New_File(sgl_GraphicsDevice* device, const char* path);
SGL_API extern sgl_Texture2D* sgl_Texture2D_New_Source(sgl_GraphicsDevice* device, void* data, size_t dataSize);

SGL_API extern void sgl_Texture2D_Destroy(sgl_Texture2D* tex);

SGL_API extern void sgl_Texture2D_Bind(sgl_Texture2D* tex, uint32 slot);

SGL_END