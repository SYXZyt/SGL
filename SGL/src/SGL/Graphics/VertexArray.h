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
    void (*Upload)(struct sgl_VertexArray* self);
} sgl_VertexArrayVTable;

typedef struct sgl_VertexArray sgl_sealed
{
    const sgl_VertexArrayVTable* vtable;
    
    sgl_GraphicsDevice* gpu;

    sgl_VertexLayout layout;

    byte* vertexData;
    word* indexData;

    uint32 vertexCount;
    uint32 indexCount;

    uint32 vertexCapacity;
    uint32 indexCapacity;

    uint32 vertexSize;

    bool isIndexed;

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

SGL_API extern sgl_VertexArray* sgl_VertexArray_Create(sgl_GraphicsDevice* gpu, uint32 vertexSize);
SGL_API extern void sgl_VertexArray_Bind(sgl_VertexArray* va);
SGL_API extern void sgl_VertexArray_Upload(sgl_VertexArray* va);
SGL_API extern void sgl_VertexArray_Destroy(sgl_VertexArray* va);

SGL_API extern void sgl_VertexArray_SetLayout(sgl_VertexArray* va);

SGL_API extern void sgl_VertexArray_Set(sgl_VertexArray* va, byte* vertices, word* indices, uint32 vertexCount, uint32 indexCount);

SGL_API extern void sgl_VertexArray_AddVertex(sgl_VertexArray* va, void* vertex);
SGL_API extern void sgl_VertexArray_AddQuad(sgl_VertexArray* va, sgl_VertexArray_Quad quad);
SGL_API extern void sgl_VertexArray_AddTri(sgl_VertexArray* va, sgl_VertexArray_Tri tri);

SGL_END