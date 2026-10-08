#include "VertexArray.h"
#include <SGL/Util/Memory.h>
#include <cstring>
#include <SGL/Graphics/Backends/OpenGL/GLVertexArray.h>
#include <SGL/Graphics/Backends/DirectX/DXVertexArray.h>
#include <SGL/Util/Error.h>

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

static void AddRawVertex(sgl_VertexArray* va, const void* v)
{
    EnsureVertexCapacity(va, va->vertexCount + 1);

    std::memcpy(va->vertexData + va->vertexCount * va->vertexSize, v, va->vertexSize);

    ++va->vertexCount;
    va->needsVertexUpload = true;
}

static void EnsureIndexCapacity(sgl_VertexArray* va, uint32 required)
{
    if (required <= va->indexCapacity)
        return;

    uint32 newCap = va->indexCapacity == 0 ? 16 : va->indexCapacity * 2;
    while (newCap < required)
        newCap *= 2;

    va->indexData = (uint32*)sgl_Realloc(va->indexData, (size_t)newCap * sizeof(uint32));
    va->indexCapacity = newCap;
}

static void EnsureInstanceCapacity(sgl_VertexArray* va, uint32 required)
{
    if (required <= va->instanceCapacity)
        return;

    uint32 newCap = va->instanceCapacity == 0 ? 16 : va->instanceCapacity * 2;
    while (newCap < required)
        newCap *= 2;

    va->instanceData = (byte*)sgl_Realloc(va->instanceData, (size_t)newCap * va->instanceSize);
    va->instanceCapacity = newCap;
}

static void AddRawInstance(sgl_VertexArray* va, const void* instance)
{
    EnsureInstanceCapacity(va, va->instanceCount + 1);

    std::memcpy(va->instanceData + va->instanceCount * va->instanceSize, instance, va->instanceSize);

    ++va->instanceCount;
    va->needsInstanceUpload = true;
}

sgl_VertexArray_Triangulated sgl_Triangulate(void* tl, void* tr, void* br, void* bl)
{
    sgl_VertexArray_Triangulated result{};

    result.t0.v0 = tl;
    result.t0.v1 = tr;
    result.t0.v2 = br;

    result.t1.v0 = tl;
    result.t1.v1 = br;
    result.t1.v2 = bl;

    return result;
}

sgl_VertexArray* sgl_VertexArray_Create(sgl_GraphicsDevice* gpu, uint32 vertexSize, sgl_VertexLayout* layout, uint32 instanceSize)
{
    sgl_VertexArray* va = nullptr;
    sgl_VertexLayout* ourLayout = sgl_VertexLayout_DeepCopy(layout);

    if (gpu->window->cfg.backend == sgl_Backend_OPENGL)
    {
        va = (sgl_VertexArray*)sgl_GLVertexArray_New(vertexSize, ourLayout, instanceSize);
    }
    else if (gpu->window->cfg.backend == sgl_Backend_DIRECTX11)
    {
#ifdef SGL_DIRECTX
        va = (sgl_VertexArray*)sgl_DXVertexArray_New(vertexSize, ourLayout, instanceSize);
#else
        SGL_REPORT_ERROR("DirectX is not supported on this platform");
        return nullptr;
#endif
    }
    else
    {
        SGL_REPORT_ERROR("Unsupported backend");
        return nullptr;
    }

    va->layout = ourLayout;
    va->gpu = gpu;
    return va;
}

void sgl_VertexArray_Bind(sgl_VertexArray* va) {
    va->vtable->Bind(va);
}

void sgl_VertexArray_Destroy(sgl_VertexArray* va)
{
    sgl_Free(va->vertexData);
    sgl_Free(va->indexData);
    sgl_Free(va->instanceData);
    va->vtable->Destroy(va);
}

void sgl_VertexArray_Set(sgl_VertexArray* va, byte* vertices, uint32 vertexCount)
{
    va->vertexCount = vertexCount;
    va->vertexCapacity = vertexCount;

    va->vertexData = (byte*)sgl_Realloc(va->vertexData, (size_t)vertexCount * va->vertexSize);
    std::memcpy(va->vertexData, vertices, (size_t)va->vertexCount * va->vertexSize);

    va->needsVertexUpload = true;
}

void sgl_VertexArray_AddVertex(sgl_VertexArray* va, void* vertex) {
    AddRawVertex(va, vertex);
}

void sgl_VertexArray_AddQuad(sgl_VertexArray* va, sgl_VertexArray_Quad quad)
{
    auto tri = sgl_Triangulate(quad.tl, quad.tr, quad.br, quad.bl);

    AddRawVertex(va, tri.t0.v0);
    AddRawVertex(va, tri.t0.v1);
    AddRawVertex(va, tri.t0.v2);

    AddRawVertex(va, tri.t1.v0);
    AddRawVertex(va, tri.t1.v1);
    AddRawVertex(va, tri.t1.v2);
}

void sgl_VertexArray_AddTri(sgl_VertexArray* va, sgl_VertexArray_Tri tri)
{
    AddRawVertex(va, tri.v0);
    AddRawVertex(va, tri.v1);
    AddRawVertex(va, tri.v2);
}

void sgl_VertexArray_SetIndices(sgl_VertexArray* va, const uint32* indices, uint32 indexCount)
{
    va->indexCount = indexCount;
    va->indexCapacity = indexCount;

    va->indexData = (uint32*)sgl_Realloc(va->indexData, (size_t)indexCount * sizeof(uint32));
    std::memcpy(va->indexData, indices, (size_t)indexCount * sizeof(uint32));

    va->needsIndexUpload = true;
}

void sgl_VertexArray_AddIndex(sgl_VertexArray* va, uint32 index)
{
    EnsureIndexCapacity(va, va->indexCount + 1);

    va->indexData[va->indexCount] = index;
    ++va->indexCount;
    va->needsIndexUpload = true;
}

void sgl_VertexArray_AddTriIndices(sgl_VertexArray* va, uint32 i0, uint32 i1, uint32 i2)
{
    sgl_VertexArray_AddIndex(va, i0);
    sgl_VertexArray_AddIndex(va, i1);
    sgl_VertexArray_AddIndex(va, i2);
}

void sgl_VertexArray_SetInstances(sgl_VertexArray* va, byte* instances, uint32 instanceCount)
{
    if (va->instanceSize == 0)
    {
        SGL_REPORT_ERROR("VertexArray was not created with instancing enabled (instanceSize == 0)");
        return;
    }

    va->instanceCount = instanceCount;
    va->instanceCapacity = instanceCount;

    va->instanceData = (byte*)sgl_Realloc(va->instanceData, (size_t)instanceCount * va->instanceSize);
    std::memcpy(va->instanceData, instances, (size_t)va->instanceCount * va->instanceSize);

    va->needsInstanceUpload = true;
}

void sgl_VertexArray_AddInstance(sgl_VertexArray* va, void* instance)
{
    if (va->instanceSize == 0)
    {
        SGL_REPORT_ERROR("VertexArray was not created with instancing enabled (instanceSize == 0)");
        return;
    }

    AddRawInstance(va, instance);
}