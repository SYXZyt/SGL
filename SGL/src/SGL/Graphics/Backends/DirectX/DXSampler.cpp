#ifdef SGL_DIRECTX
#include "DXSampler.h"
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

#define GetSelf sgl_DXSampler* self = (sgl_DXSampler*)sampler

static void DXEnsureGPUResources(sgl_DXSampler* self)
{
    self->base.gpuLoaded = true;

    D3D11_SAMPLER_DESC desc = {};

    switch (self->base.filter)
    {
        case sgl_TextureFilter_LINEAR:
            desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
            break;
        case sgl_TextureFilter_NEAREST:
        default:
            desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
            break;
    }

    switch (self->base.clamp)
    {
        case sgl_TextureClamp_REPEAT:
            desc.AddressU = desc.AddressV = desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
            break;
        case sgl_TextureClamp_MIRROR:
            desc.AddressU = desc.AddressV = desc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
            break;
        case sgl_TextureClamp_EDGE:
        default:
            desc.AddressU = desc.AddressV = desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
            break;
    }

    desc.ComparisonFunc = D3D11_COMPARISON_NEVER;

    desc.MinLOD = 0;
    desc.MaxLOD = D3D11_FLOAT32_MAX;

    sgl_DXDevice* dev = (sgl_DXDevice*)self->base.gpu;
    HRESULT hr = dev->device->CreateSamplerState(&desc, &self->samplerstate);

    if (FAILED(hr))
    {
        ReportHRError("Failed to create sampler", hr);
        return;
    }
}

static void DXDestroy(sgl_Sampler* sampler)
{
    GetSelf;
    self->samplerstate->Release();
    sgl::Memory::Delete(self);
}

static void DXBind(sgl_Sampler* sampler, uint32 unit)
{
    GetSelf;

    if (!sampler->gpuLoaded)
        DXEnsureGPUResources(self);

    sgl_DXDevice* device = (sgl_DXDevice*)sampler->gpu;
    device->ctx->PSSetSamplers(unit, 1, &self->samplerstate);
}

static const sgl_SamplerVTable gDXVTable =
{
    .Destroy = &DXDestroy,
    .Bind = &DXBind,
};

sgl_DXSampler* sgl_DXSampler_Create(sgl_TextureFilter filter, sgl_TextureClamp clamp)
{
    sgl_DXSampler* sampler = sgl::Memory::New<sgl_DXSampler>();

    sampler->base.clamp = clamp;
    sampler->base.filter = filter;
    sampler->base.vtable = &gDXVTable;
    sampler->base.gpuLoaded = false;

    return sampler;
}
#endif