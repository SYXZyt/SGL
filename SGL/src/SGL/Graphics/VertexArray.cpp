#include "VertexArray.h"
#include <SGL/Util/Memory.h>
#include <cstring>

static void EnsureVertexCapacity(sgl_VertexArray* va, uint32 required)
{
    if (required <= va->vertexCapacity)
        return;

    uint32 newCap = va->vertexCapacity == 0 ? 16 : va->vertexCapacity * 2;
    while (newCap < required)
        newCap *= 2;

    va->vertexData = (byte*)sgl_Realloc(va->vertexData, (size_t)newCap * va->vertexSize);
    va->vertexCapacity = newCap;
}

static void EnsureIndexCapacity(sgl_VertexArray* va, uint32 required)
{
    if (required <= va->indexCapacity)
        return;

    uint32 newCap = va->indexCapacity == 0 ? 16 : va->indexCapacity * 2;
    while (newCap < required)
        newCap *= 2;

    va->indexData = (word*)sgl_Realloc(va->indexData, (size_t)newCap * sizeof(word));
    va->indexCapacity = newCap;
}

static void AddRawVertex(sgl_VertexArray* va, const void* v)
{
    EnsureVertexCapacity(va, va->vertexCount + 1);

    std::memcpy(va->vertexData + va->vertexCount * va->vertexSize, v, va->vertexSize);

    ++va->vertexCount;
    va->needsVertexUpload = true;
}

sgl_VertexArray_Triangulated sgl_Triangulate(void* tl, void* tr, void* br, void* bl)
{
    sgl_VertexArray_Triangulated result{};

    result.t0.v0 = tl;
    result.t0.v1 = tr;
    result.t0.v2 = br;

    result.t1.v0 = tl;
    result.t1.v0 = br;
    result.t1.v0 = bl;

    return result;
}

sgl_VertexArray* sgl_VertexArray_Create(sgl_GraphicsDevice* gpu, uint32 vertexSize)
{
    return nullptr;
}

void sgl_VertexArray_Bind(sgl_VertexArray* va) {
    va->vtable->Bind(va);
}

void sgl_VertexArray_Upload(sgl_VertexArray* va) {
    va->vtable->Upload(va);
}

void sgl_VertexArray_Destroy(sgl_VertexArray* va) {
    va->vtable->Destroy(va);
}

void sgl_VertexArray_Set(sgl_VertexArray* va, byte* vertices, word* indices, uint32 vertexCount, uint32 indexCount)
{
    va->vertexCount = vertexCount;
    va->indexCount = indexCount;

    sgl_Realloc(va->vertexData, (size_t)vertexCount * va->vertexSize);
    sgl_Realloc(va->indexData, (size_t)indexCount * sizeof(word));

    va->vertexCapacity = vertexCount;
    va->indexCapacity = indexCount;

    std::memcpy(va->vertexData, vertices, (size_t)va->vertexCount * va->vertexSize);
    std::memcpy(va->indexData, indices, (size_t)indexCount * sizeof(word));
}

void sgl_VertexArray_AddVertex(sgl_VertexArray* va, void* vertex)
{
}

void sgl_VertexArray_AddQuad(sgl_VertexArray* va, sgl_VertexArray_Quad quad)
{
}

void sgl_VertexArray_AddTri(sgl_VertexArray* va, sgl_VertexArray_Tri tri)
{
}