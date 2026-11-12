#include "DXFramebuffer.h"
#ifdef SGL_DIRECTX
#include <SGL/Util/Memory.h>
#include <SGL/Util/Error.h>
#include <SGL/Graphics/Texture.h>
#include <SGL/Graphics/Sampler.h>
#include <SGL/Graphics/Backends/DirectX/DXDevice.h>
#include <sstream>

#define GetSelf sgl_DXFramebuffer* self = (sgl_DXFramebuffer*)fb

static void ReportHRError(const char* message, HRESULT hr)
{
    std::stringstream ss;
    ss << message << " (" << sgl_HResultToString(hr) << ')';
    SGL_REPORT_ERROR(ss.str().c_str());
}

typedef struct sgl_DXFramebufferTexture sgl_sealed
{
    sgl_Texture base;
    sgl_DXFramebuffer* owner;
} sgl_DXFramebufferTexture;

static void DXFramebufferTexture_Destroy(sgl_Texture*) {
}

static void DXFramebufferTexture_Bind(sgl_Texture* tex, uint32 unit)
{
    sgl_DXFramebufferTexture* self = (sgl_DXFramebufferTexture*)tex;
    sgl_DXDevice* device = (sgl_DXDevice*)tex->gpu;

    device->ctx->PSSetShaderResources(unit, 1, &self->owner->srv);
    sgl_Sampler_Bind(tex->sampler, unit);
}

static sgl_TextureVTable gDXFramebufferTextureVTable =
{
    .Destroy = &DXFramebufferTexture_Destroy,
    .Bind = &DXFramebufferTexture_Bind,
};

static void DX_ReleaseTargets(sgl_DXFramebuffer* self)
{
    if (self->srv)
    {
        self->srv->Release();
        self->srv = nullptr;
    }

    if (self->rtv)
    {
        self->rtv->Release();
        self->rtv = nullptr;
    }

    if (self->texture)
    {
        self->texture->Release();
        self->texture = nullptr;
    }

    if (self->dsv)
    {
        self->dsv->Release();
        self->dsv = nullptr;
    }

    if (self->depthTexture)
    {
        self->depthTexture->Release();
        self->depthTexture = nullptr;
    }
}

static DXGI_FORMAT DX_ToFormat(sgl_FramebufferFormat format)
{
    switch (format)
    {
        case sgl_FramebufferFormat_RGBA16F: return DXGI_FORMAT_R16G16B16A16_FLOAT;
        case sgl_FramebufferFormat_R8: return DXGI_FORMAT_R8_UNORM;
        case sgl_FramebufferFormat_RGBA8:
        default: return DXGI_FORMAT_R8G8B8A8_UNORM;
    }
}

static void DX_CreateTargets(sgl_DXFramebuffer* self, sgl_Vec2i size)
{
    sgl_DXDevice* device = (sgl_DXDevice*)self->base.gpu;

    D3D11_TEXTURE2D_DESC desc{};
    desc.Width = (UINT)size.width;
    desc.Height = (UINT)size.height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DX_ToFormat(self->base.format);
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

    HRESULT hr = device->device->CreateTexture2D(&desc, nullptr, &self->texture);
    if (FAILED(hr))
    {
        ReportHRError("Failed to create framebuffer texture", hr);
        return;
    }

    hr = device->device->CreateRenderTargetView(self->texture, nullptr, &self->rtv);
    if (FAILED(hr))
    {
        ReportHRError("Failed to create framebuffer RTV", hr);
        return;
    }

    hr = device->device->CreateShaderResourceView(self->texture, nullptr, &self->srv);
    if (FAILED(hr))
    {
        ReportHRError("Failed to create framebuffer SRV", hr);
        return;
    }

    if (self->base.hasDepth)
    {
        D3D11_TEXTURE2D_DESC depthDesc{};
        depthDesc.Width = (UINT)size.width;
        depthDesc.Height = (UINT)size.height;
        depthDesc.MipLevels = 1;
        depthDesc.ArraySize = 1;
        depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthDesc.SampleDesc.Count = 1;
        depthDesc.Usage = D3D11_USAGE_DEFAULT;
        depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        hr = device->device->CreateTexture2D(&depthDesc, nullptr, &self->depthTexture);
        if (FAILED(hr))
        {
            ReportHRError("Failed to create framebuffer depth texture", hr);
            return;
        }

        hr = device->device->CreateDepthStencilView(self->depthTexture, nullptr, &self->dsv);
        if (FAILED(hr))
        {
            ReportHRError("Failed to create framebuffer DSV", hr);
            return;
        }
    }
}

static void DX_Destroy(sgl_Framebuffer* fb)
{
    GetSelf;
    DX_ReleaseTargets(self);
    sgl::Memory::Delete((sgl_DXFramebufferTexture*)self->base.colourTexture);
    sgl::Memory::Delete(self);
}

static void DX_Resize(sgl_Framebuffer* fb, sgl_Vec2i size)
{
    GetSelf;
    DX_ReleaseTargets(self);
    self->base.size = size;
    self->base.colourTexture->size = size;
    DX_CreateTargets(self, size);
}

static void DX_Bind(sgl_Framebuffer* fb)
{
    GetSelf;
    sgl_DXDevice* device = (sgl_DXDevice*)fb->gpu;

    if (device->boundTextureCount > 0)
    {
        static ID3D11ShaderResourceView* const nullSrvs[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = {};
        device->ctx->PSSetShaderResources(0, (UINT)device->boundTextureCount, nullSrvs);
        device->boundTextureCount = 0;
    }

    device->ctx->OMSetRenderTargets(1, &self->rtv, self->dsv);

    D3D11_VIEWPORT viewport =
    {
        .Width = (FLOAT)fb->size.width,
        .Height = (FLOAT)fb->size.height,
        .MinDepth = 0.0f,
        .MaxDepth = 1.0f,
    };

    device->ctx->RSSetViewports(1, &viewport);
}

static void DX_Clear(sgl_Framebuffer* fb, sgl_Colour colour)
{
    GetSelf;
    sgl_DXDevice* device = (sgl_DXDevice*)fb->gpu;

    device->ctx->ClearRenderTargetView(self->rtv, &colour.r);

    if (self->dsv)
        device->ctx->ClearDepthStencilView(self->dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

static sgl_FramebufferVTable gDXVTable =
{
    .Destroy = &DX_Destroy,
    .Resize = &DX_Resize,
    .Bind = &DX_Bind,
    .Clear = &DX_Clear,
};

sgl_DXFramebuffer* sgl_DXFramebuffer_Create(sgl_GraphicsDevice* device, sgl_Vec2i size, bool hasDepth, sgl_FramebufferFormat format)
{
    sgl_DXFramebuffer* framebuffer = sgl::Memory::New<sgl_DXFramebuffer>();
    framebuffer->base.vtable = &gDXVTable;
    framebuffer->base.gpu = device;
    framebuffer->base.size = size;
    framebuffer->base.hasDepth = hasDepth;
    framebuffer->base.format = format;

    DX_CreateTargets(framebuffer, size);

    sgl_DXFramebufferTexture* texture = sgl::Memory::New<sgl_DXFramebufferTexture>();
    texture->base.vtable = &gDXFramebufferTextureVTable;
    texture->base.gpu = device;
    texture->base.sampler = nullptr;
    texture->base.size = size;
    texture->base.gpuLoaded = true;
    texture->owner = framebuffer;

    framebuffer->base.colourTexture = (sgl_Texture*)texture;

    return framebuffer;
}

#endif