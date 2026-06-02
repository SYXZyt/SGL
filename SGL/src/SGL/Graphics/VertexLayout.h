#pragma once
#include <SGL/SGL.h>
#include <SGL/Graphics/GraphicsDevice.h>
#include <SGL/Util/String.h>

SGL_BEGIN

typedef uint8 sgl_VertexElementType;
enum
{
    sgl_VertexElementType_FLOAT,

    sgl_VertexElementType_INT8,
    sgl_VertexElementType_INT16,
    sgl_VertexElementType_INT32,

    sgl_VertexElementType_UINT8,
    sgl_VertexElementType_UINT16,
    sgl_VertexElementType_UINT32,

    sgl_VertexElementType_VEC2,
    sgl_VertexElementType_VEC3,
    sgl_VertexElementType_VEC4,
};

typedef struct sgl_VertexElement sgl_sealed
{
    sgl_String semantic;
    size_t offset;
    sgl_VertexElementType type;
    bool perInstance; // Unused for now    
} sgl_VertexElement;

typedef struct sgl_VertexLayout sgl_sealed
{
    sgl_GraphicsDevice* gpu;

    sgl_VertexElement* elements;
    uint32 elementCount;
    uint32 elementCapacity;
} sgl_VertexLayout;

SGL_API extern sgl_VertexLayout* sgl_VertexLayout_New(sgl_GraphicsDevice* gpu);
SGL_API extern sgl_VertexLayout* sgl_VertexLayout_DeepCopy(sgl_VertexLayout* layout);
SGL_API extern void sgl_VertexLayout_Add(sgl_VertexLayout* layout, sgl_VertexElement element);
SGL_API extern void sgl_VertexLayout_Destroy(sgl_VertexLayout* layout);

SGL_END