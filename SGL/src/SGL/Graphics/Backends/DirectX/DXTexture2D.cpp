#ifdef SGL_DIRECTX

#include "DXTexture2D.h"
#include <SGL/Util/Memory.h>
#include <SGL/Graphics/Backends/DirectX/DXDevice.h>
#include <SGL/Util/Error.h>
#include <stb/stb_image.h>
#include <sstream>

static void ReportHRError(const char* message, HRESULT hr)
{
    std::stringstream ss;
    ss << message << " (" << sgl_HResultToString(hr) << ')';
    SGL_REPORT_ERROR(ss.str().c_str());
}

#define GetSelf sgl_DXTexture2D* self = (sgl_DXTexture2D*)tex

static void DXTexture_Destroy(sgl_Texture* tex)
{
    GetSelf;

    if (self->texture)
        self->texture->Release();

    if (self->textureView)
        self->textureView->Release();

    if (self->base.pixels)
        stbi_image_free(self->base.pixels);

    sgl::Memory::Delete(self);
}

static void DXEnsureGPUResources(sgl_DXTexture2D* self)
{
    sgl_Texture* tex = &self->base.base;
    sgl_DXDevice* device = (sgl_DXDevice*)tex->gpu;

    D3D11_TEXTURE2D_DESC desc{};
    desc.Width = tex->size.width;
    desc.Height = tex->size.height;
    desc.MipLevels = self->generateMipmaps ? 0 : 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | (self->generateMipmaps ? D3D11_BIND_RENDER_TARGET : 0);
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = self->generateMipmaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

    HRESULT hr = device->device->CreateTexture2D(&desc, nullptr, &self->texture);

    if (FAILED(hr))
    {
        ReportHRError("Failed to create texture", hr);
        stbi_image_free(self->base.pixels);
        self->base.pixels = nullptr;
        tex->gpuLoaded = true;
        return;
    }

    device->ctx->UpdateSubresource(self->texture, 0, nullptr, self->base.pixels, tex->size.width * 4, 0);

    stbi_image_free(self->base.pixels);
    self->base.pixels = nullptr;

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = (UINT)-1;

    hr = device->device->CreateShaderResourceView(
        self->texture,
        &srvDesc,
        &self->textureView);

    if (FAILED(hr))
    {
        ReportHRError("Failed to create shader resource view", hr);
        tex->gpuLoaded = true;
        return;
    }

    if (self->generateMipmaps)
        device->ctx->GenerateMips(self->textureView);

    tex->gpuLoaded = true;
}

static void DXTexture_Bind(sgl_Texture* tex, uint32 unit)
{
    GetSelf;

    if (!tex->gpuLoaded)
        DXEnsureGPUResources(self);

    sgl_DXDevice* device = (sgl_DXDevice*)tex->gpu;
    device->ctx->PSSetShaderResources(unit, 1, &self->textureView);
    sgl_Sampler_Bind(tex->sampler, unit);
}

static sgl_TextureVTable gDXVTable =
{
    .Destroy = &DXTexture_Destroy,
    .Bind = &DXTexture_Bind,
};

sgl_DXTexture2D* sgl_DXTexture2D_Create(void* data, sgl_Vec2i size, bool generateMipmaps)
{
    sgl_DXTexture2D* texture = sgl::Memory::New<sgl_DXTexture2D>();
    texture->base.base.size = size;
    texture->base.base.vtable = &gDXVTable;
    texture->base.base.gpuLoaded = false;
    texture->base.pixels = data;
    texture->texture = nullptr;
    texture->textureView = nullptr;
    texture->generateMipmaps = generateMipmaps;

    return texture;
}

#endif