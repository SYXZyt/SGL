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

    if (self->indexBuffer)
        self->indexBuffer->Release();

    if (self->instanceBuffer)
        self->instanceBuffer->Release();

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

    if (va->needsIndexUpload)
    {
        if (self->indexBuffer)
        {
            self->indexBuffer->Release();
            self->indexBuffer = nullptr;
        }

        if (va->indexCount > 0)
        {
            D3D11_BUFFER_DESC desc = {};
            desc.ByteWidth = (UINT)(va->indexCount * sizeof(uint32));
            desc.Usage = D3D11_USAGE_DEFAULT;
            desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

            D3D11_SUBRESOURCE_DATA data = {};
            data.pSysMem = va->indexData;

            HRESULT hr = device->device->CreateBuffer(&desc, &data, &self->indexBuffer);
            if (FAILED(hr))
                ReportHRError("Failed to create index buffer", hr);
        }

        va->needsIndexUpload = false;
    }

    if (va->needsInstanceUpload)
    {
        if (self->instanceBuffer)
        {
            self->instanceBuffer->Release();
            self->instanceBuffer = nullptr;
        }

        if (va->instanceCount > 0 && va->instanceSize > 0)
        {
            D3D11_BUFFER_DESC desc = {};
            desc.ByteWidth = (UINT)(va->instanceCount * va->instanceSize);
            desc.Usage = D3D11_USAGE_DEFAULT;
            desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

            D3D11_SUBRESOURCE_DATA data = {};
            data.pSysMem = va->instanceData;

            HRESULT hr = device->device->CreateBuffer(&desc, &data, &self->instanceBuffer);
            if (FAILED(hr))
                ReportHRError("Failed to create instance buffer", hr);
        }

        va->needsInstanceUpload = false;
    }

    if (self->instanceBuffer)
    {
        ID3D11Buffer* buffers[2] = { self->vertexBuffer, self->instanceBuffer };
        UINT strides[2] = { va->vertexSize, va->instanceSize };
        UINT offsets[2] = { 0, 0 };
        device->ctx->IASetVertexBuffers(0, 2, buffers, strides, offsets);
    }
    else
    {
        UINT stride = va->vertexSize;
        UINT offset = 0;
        device->ctx->IASetVertexBuffers(0, 1, &self->vertexBuffer, &stride, &offset);
    }

    if (self->indexBuffer)
        device->ctx->IASetIndexBuffer(self->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    device->ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

static sgl_VertexArrayVTable gDXVtable =
{
    .Destroy = &DXDestroy,
    .Bind = &DXBind,
};

sgl_DXVertexArray* sgl_DXVertexArray_New(uint32 vertexSize, sgl_VertexLayout* layout, uint32 instanceSize)
{
    sgl_DXVertexArray* va = sgl::Memory::New<sgl_DXVertexArray>();

    va->base.vtable = &gDXVtable;

    va->base.vertexData = nullptr;
    va->base.vertexCount = 0;
    va->base.vertexCapacity = 0;
    va->base.vertexSize = vertexSize;

    va->base.indexData = nullptr;
    va->base.indexCount = 0;
    va->base.indexCapacity = 0;

    va->base.instanceData = nullptr;
    va->base.instanceCount = 0;
    va->base.instanceCapacity = 0;
    va->base.instanceSize = instanceSize;

    va->base.needsVertexUpload = true;
    va->base.needsIndexUpload = true;
    va->base.needsInstanceUpload = true;
    va->base.layoutDirty = true;

    va->vertexBuffer = nullptr;
    va->indexBuffer = nullptr;
    va->instanceBuffer = nullptr;

    return va;
}

#endif