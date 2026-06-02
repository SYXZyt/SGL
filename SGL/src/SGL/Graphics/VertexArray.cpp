#include "VertexArray.h"
#include <SGL/Util/Memory.h>
#include <cstring>
#include <SGL/Graphics/Backends/OpenGL/GLVertexArray.h>
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

sgl_VertexArray* sgl_VertexArray_Create(sgl_GraphicsDevice* gpu, uint32 vertexSize, sgl_VertexLayout* layout)
{
    sgl_VertexArray* va = nullptr;
    sgl_VertexLayout* ourLayout = sgl_VertexLayout_DeepCopy(layout);

    if (gpu->window->cfg.backend == sgl_Backend_OPENGL)
        va = (sgl_VertexArray*)sgl_GLVertexArray_New(vertexSize, ourLayout);
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

void sgl_VertexArray_Destroy(sgl_VertexArray* va) {
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