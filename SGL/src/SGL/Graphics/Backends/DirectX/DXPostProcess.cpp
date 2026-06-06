#include "DXPostProcess.h"
#ifdef SGL_DIRECTX
#include <SGL/Util/Memory.h>
#include <SGL/Util/Error.h>
#include <sstream>
#include <SGL/Graphics/Backends/DirectX/DXDevice.h>

#define GetSelf sgl_DXPostProcess* self = (sgl_DXPostProcess*)pp

static void ReportHRError(const char* message, HRESULT hr)
{
    std::stringstream ss;
    ss << message << " (" << sgl_HResultToString(hr) << ')';
    SGL_REPORT_ERROR(ss.str().c_str());
}

static void DX_Release(sgl_DXPostProcess* pp)
{
    if (pp->texture)
    {
        pp->texture->Release();
        pp->texture = nullptr;
    }

    if (pp->renderTargetView)
    {
        pp->renderTargetView->Release();
        pp->renderTargetView = nullptr;
    }

    if (pp->shaderResourceView)
    {
        pp->shaderResourceView->Release();
        pp->shaderResourceView = nullptr;
    }
}

static void DX_Init(sgl_DXPostProcess* pp, sgl_Vec2i screenSize, sgl_DXDevice* device)
{
    D3D11_TEXTURE2D_DESC desc{};
    desc.Width = screenSize.width;
    desc.Height = screenSize.height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

    HRESULT hr = device->device->CreateTexture2D(&desc, nullptr, &pp->texture);
    if (FAILED(hr))
    {
        ReportHRError("Failed to create texture", hr);
        return;
    }

    hr = device->device->CreateRenderTargetView(pp->texture, nullptr, &pp->renderTargetView);
    if (FAILED(hr))
    {
        ReportHRError("Failed to create render target", hr);
        return;
    }

    hr = device->device->CreateShaderResourceView(pp->texture, nullptr, &pp->shaderResourceView);
    if (FAILED(hr))
    {
        ReportHRError("Failed to create shader resource", hr);
        return;
    }
}

static void DX_Bind(sgl_PostProcess* pp) {
    sgl_Shader_Bind(pp->shader);
}

static void DX_Destroy(sgl_PostProcess* pp)
{
    GetSelf;
    DX_Release(self);
    sgl_Shader_Destroy(pp->shader);
    sgl::Memory::Delete(self);
}

static void DX_OnResize(sgl_PostProcess* pp)
{
    GetSelf;
    DX_Release(self);
    DX_Init(self, pp->gpu->window->screenSize, (sgl_DXDevice*)pp->gpu);
}

static sgl_PostProcessVTable gDXVTable =
{
    .Destroy = &DX_Destroy,
    .Bind = &DX_Bind,
    .OnResize = &DX_OnResize,
};

sgl_DXPostProcess* sgl_DXPostProcess_Create(sgl_GraphicsDevice* device)
{
    sgl_DXPostProcess* pp = sgl::Memory::New<sgl_DXPostProcess>();
    pp->base.enabled = true;
    pp->base.vtable = &gDXVTable;

    DX_Init(pp, device->window->screenSize, (sgl_DXDevice*)device);

    return pp;
}

#endif