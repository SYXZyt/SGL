#pragma once
#include <SGL/SGL.h>
#include <SGL/Graphics/GraphicsDevice.h>

SGL_BEGIN

typedef uint8 sgl_VertexElementType;
enum
{
    sgl_VertexElementType_FLOAT,
    sgl_VertexElementType_FLOAT2,
    sgl_VertexElementType_FLOAT3,
    sgl_VertexElementType_FLOAT4,
    sgl_VertexElementType_UINT,
    sgl_VertexElementType_UINT2,
    sgl_VertexElementType_UINT3,
    sgl_VertexElementType_UINT4,
};

typedef struct sgl_VertexElement sgl_sealed
{
    uint32 index;
    uint32 count;
    uint32 offset;
    uint32 strideBytes;
    bool normalised;
    bool integer;
} sgl_VertexElement;

struct sgl_VertexLayout;
typedef struct sgl_VertexLayoutVTable sgl_sealed
{
    void (*Destroy)(struct sgl_VertexLayout* self);
    void (*Add)(struct sgl_VertexLayout* self, sgl_VertexElementType type);
    void (*Bind)(struct sgl_VertexLayout* self);
} sgl_VertexLayoutVTable;

typedef struct sgl_VertexLayout sgl_sealed
{
    const sgl_VertexLayoutVTable* vtable;

    sgl_GraphicsDevice* gpu;

    sgl_VertexElement* elements;
    uint32 elementCount;
    uint32 elementCapacity;

    uint32 stride;
    uint32 nextIndex;
} sgl_VertexLayout;

SGL_API extern sgl_VertexLayout* sgl_VertexLayout_New(sgl_GraphicsDevice* gpu);
SGL_API extern void sgl_VertexLayout_Add(sgl_VertexLayout* layout, sgl_VertexElementType type);
SGL_API extern void sgl_VertexLayout_Destroy(sgl_VertexLayout* layout);
SGL_API extern void sgl_VertexLayout_Bind(sgl_VertexLayout* layout);

SGL_API extern uint32 sgl_VertexLayout_GetTypeSize(sgl_VertexElementType type);
SGL_API extern uint32 sgl_VertexLayout_GetComponentCount(sgl_VertexElementType type);

SGL_END