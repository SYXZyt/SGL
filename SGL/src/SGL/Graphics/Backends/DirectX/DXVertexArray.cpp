#include "DXVertexArray.h"
#ifdef SGL_DIRECTX
#include <SGL/Util/Memory.h>
#include <SGL/Graphics/Backends/DirectX/DXDevice.h>
#include <SGL/Util/Error.h>
#include <sstream>

#define GetSelf sgl_DXVertexArray* self = (sgl_DXVertexArray*)va

static void ReportHRError(const char* message, HRESULT hr)
{
    std::stringstream ss;
    ss << message << " (" << sgl_HResultToString(hr) << ')';
    SGL_REPORT_ERROR(ss.str().c_str());
}


static void DXDestroy(sgl_VertexArray* va)
{
    GetSelf;

    if (self->vertexBuffer)
        self->vertexBuffer->Release();

    sgl_VertexLayout_Destroy(va->layout);
    sgl::Memory::Delete(self);
}

static void DXBind(sgl_VertexArray* va)
{
    GetSelf;

    sgl_DXDevice* device = (sgl_DXDevice*)va->gpu;

    if (va->needsVertexUpload)
    {
        if (self->vertexBuffer)
        {
            self->vertexBuffer->Release();
            self->vertexBuffer = nullptr;
        }

        if (va->vertexCount == 0)
            return;

        D3D11_BUFFER_DESC desc = {};
        desc.ByteWidth = (UINT)(va->vertexCount * va->vertexSize);
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA data = {};
        data.pSysMem = va->vertexData;

        HRESULT hr = device->device->CreateBuffer(&desc, &data, &self->vertexBuffer);
        if (FAILED(hr))
        {
            ReportHRError("Failed to create buffer", hr);
            return;
        }

        va->needsVertexUpload = false;
    }

    UINT stride = va->vertexSize;
    UINT offset = 0;

    device->ctx->IASetVertexBuffers(0, 1, &self->vertexBuffer, &stride, &offset);

    device->ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

static sgl_VertexArrayVTable gDXVtable =
{
    .Destroy = &DXDestroy,
    .Bind = &DXBind,
};

sgl_DXVertexArray* sgl_DXVertexArray_New(uint32 vertexSize, sgl_VertexLayout* layout)
{
    sgl_DXVertexArray* va = sgl::Memory::New<sgl_DXVertexArray>();

    va->base.vtable = &gDXVtable;

    va->base.vertexData = nullptr;
    va->base.vertexCount = 0;
    va->base.vertexCapacity = 0;
    va->base.vertexSize = vertexSize;

    va->base.needsVertexUpload = true;
    va->base.layoutDirty = true;

    va->vertexBuffer = nullptr;

    return va;
}

#endif