#include "DXUniformBuffer.h"
#ifdef SGL_DIRECTX
#include <SGL/Util/Memory.h>
#include <SGL/Graphics/Backends/DirectX/DXDevice.h>
#include <SGL/Util/Error.h>
#include <sstream>

static void ReportHRError(const char* message, HRESULT hr)
{
    std::stringstream ss;
    ss << message << " (" << sgl_HResultToString(hr) << ')';
    SGL_REPORT_ERROR(ss.str().c_str());
}

#define GetSelf sgl_DXUniformBuffer* self = (sgl_DXUniformBuffer*)ub

static void DXDestroy(sgl_UniformBuffer* ub)
{
    GetSelf;

    if (self->buffer)
        self->buffer->Release();

    sgl::Memory::Delete(self);
}

static void DXBind(sgl_UniformBuffer* ub, uint32 slot)
{
    GetSelf;

    sgl_DXDevice* device = (sgl_DXDevice*)ub->gpu;
    device->ctx->VSSetConstantBuffers(slot, 1, &self->buffer);
    device->ctx->PSSetConstantBuffers(slot, 1, &self->buffer);
}

static void DXUpload(sgl_UniformBuffer* ub, void* data)
{
    GetSelf;

    sgl_DXDevice* device = (sgl_DXDevice*)ub->gpu;

    D3D11_MAPPED_SUBRESOURCE mapped;
    HRESULT hr = device->ctx->Map(self->buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);

    if (FAILED(hr))
    {
        ReportHRError("Failed to map uniform buffer", hr);
        return;
    }

    std::memcpy(mapped.pData, data, self->base.size);
    device->ctx->Unmap(self->buffer, 0);
}

static sgl_UniformBufferVTable gDXVTable =
{
    .Destroy = &DXDestroy,
    .Bind = &DXBind,
    .Upload = &DXUpload,
};

sgl_DXUniformBuffer* sgl_DXUniformBuffer_New(sgl_GraphicsDevice* gpu, size_t size)
{
    sgl_DXUniformBuffer* ub = sgl::Memory::New<sgl_DXUniformBuffer>();
    ub->base.vtable = &gDXVTable;
    ub->base.size = size;

    D3D11_BUFFER_DESC desc{};
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.ByteWidth = (UINT)size;

    sgl_DXDevice* device = (sgl_DXDevice*)gpu;

    HRESULT hr = device->device->CreateBuffer(&desc, nullptr, &ub->buffer);
    if (FAILED(hr))
    {
        ReportHRError("Failed to create uniform buffer", hr);
        return ub;
    }

    return ub;
}

#endif