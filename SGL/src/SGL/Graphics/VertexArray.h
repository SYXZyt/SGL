#pragma once
#include <SGL/SGL.h>
#include <SGL/Graphics/VertexLayout.h>
#include <SGL/Graphics/GraphicsDevice.h>

SGL_BEGIN

struct sgl_VertexArray;
typedef struct sgl_VertexArrayVTable sgl_sealed
{
    void (*Destroy)(struct sgl_VertexArray* self);
    void (*Bind)(struct sgl_VertexArray* self);
} sgl_VertexArrayVTable;

typedef struct sgl_VertexArray sgl_sealed
{
    const sgl_VertexArrayVTable* vtable;
    
    sgl_GraphicsDevice* gpu;

    sgl_VertexLayout* layout;

    byte* vertexData;
    uint32 vertexCount;
    uint32 vertexCapacity;
    uint32 vertexSize;

    // If indexCount is 0, backends draw with the non-indexed path
    uint32* indexData;
    uint32 indexCount;
    uint32 indexCapacity;

    bool needsVertexUpload;
    bool needsIndexUpload;
    bool layoutDirty;
} sgl_VertexArray;

typedef struct sgl_VertexArray_Quad sgl_sealed
{
    void* tl;
    void* tr;
    void* bl;
    void* br;
} sgl_VertexArray_Quad;

typedef struct sgl_VertexArray_Tri sgl_sealed
{
    void* v0;
    void* v1;
    void* v2;
} sgl_VertexArray_Tri;

typedef struct sgl_VertexArray_Triangulated sgl_sealed
{
    sgl_VertexArray_Tri t0;
    sgl_VertexArray_Tri t1;
} sgl_VertexArray_Triangulated;

SGL_API extern sgl_VertexArray_Triangulated sgl_Triangulate(void* tl, void* tr, void* br, void* bl);

SGL_API extern sgl_VertexArray* sgl_VertexArray_Create(sgl_GraphicsDevice* gpu, uint32 vertexSize, sgl_VertexLayout* layout);
SGL_API extern void sgl_VertexArray_Bind(sgl_VertexArray* va);
SGL_API extern void sgl_VertexArray_Destroy(sgl_VertexArray* va);

SGL_API extern void sgl_VertexArray_Set(sgl_VertexArray* va, byte* vertices, uint32 vertexCount);

SGL_API extern void sgl_VertexArray_AddVertex(sgl_VertexArray* va, void* vertex);
SGL_API extern void sgl_VertexArray_AddQuad(sgl_VertexArray* va, sgl_VertexArray_Quad quad);
SGL_API extern void sgl_VertexArray_AddTri(sgl_VertexArray* va, sgl_VertexArray_Tri tri);

SGL_API extern void sgl_VertexArray_SetIndices(sgl_VertexArray* va, const uint32* indices, uint32 indexCount);
SGL_API extern void sgl_VertexArray_AddIndex(sgl_VertexArray* va, uint32 index);
SGL_API extern void sgl_VertexArray_AddTriIndices(sgl_VertexArray* va, uint32 i0, uint32 i1, uint32 i2);

SGL_END