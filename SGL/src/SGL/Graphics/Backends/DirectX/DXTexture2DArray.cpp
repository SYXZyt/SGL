#include "DXTexture2DArray.h"
#ifdef SGL_DIRECTX
#include <SGL/Util/Error.h>
#include <SGL/Util/Memory.h>
#include <stb/stb_image.h>
#include <sstream>
#include <SGL/Graphics/Backends/DirectX/DXDevice.h>
#include <vector>

static void ReportHRError(const char* message, HRESULT hr)
{
    std::stringstream ss;
    ss << message << " (" << sgl_HResultToString(hr) << ')';
    SGL_REPORT_ERROR(ss.str().c_str());
}

#define GetSelf sgl_DXTexture2DArray* self = (sgl_DXTexture2DArray*)tex

static void DXDestroy(sgl_Texture* tex)
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

static void DXEnsureGPUResources(sgl_DXTexture2DArray* self)
{
    sgl_Texture* tex = &self->base.base;
    sgl_Vec2i atlasSize = tex->size;
    sgl_Vec2i frameSize = self->base.frameSize;
    uint32 sliceCount = self->base.textureCount;

    uint32 cols = atlasSize.width / frameSize.width;
    uint32 rows = atlasSize.height / frameSize.height;

    D3D11_TEXTURE2D_DESC desc{};
    desc.Width = frameSize.width;
    desc.Height = frameSize.height;
    desc.MipLevels = 0;
    desc.ArraySize = sliceCount;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

    sgl_DXDevice* dxDevice = (sgl_DXDevice*)tex->gpu;

    HRESULT hr = dxDevice->device->CreateTexture2D(&desc, nullptr, &self->texture);

    if (FAILED(hr))
    {
        ReportHRError("Failed to create texture", hr);
        stbi_image_free(self->base.pixels);
        self->base.pixels = nullptr;
        tex->gpuLoaded = true;
        return;
    }

    D3D11_TEXTURE2D_DESC actualDesc{};
    self->texture->GetDesc(&actualDesc);

    const byte* bytes = (const byte*)self->base.pixels;
    std::vector<byte> sliceBuffer((size_t)frameSize.width * frameSize.height * 4ull);
    uint32 sliceIndex = 0;

    for (uint32 y = 0; y < rows; ++y)
    {
        for (uint32 x = 0; x < cols; ++x)
        {
            for (uint32 row = 0; row < (uint32)frameSize.height; ++row)
            {
                byte* dest = sliceBuffer.data() + row * frameSize.width * 4;
                const byte* src = bytes + ((y * frameSize.height + row) * atlasSize.width + x * frameSize.width) * 4;
                std::memcpy(dest, src, frameSize.width * 4ull);
            }

            UINT subresource = D3D11CalcSubresource(0, sliceIndex, actualDesc.MipLevels);
            dxDevice->ctx->UpdateSubresource(self->texture, subresource, nullptr, sliceBuffer.data(), frameSize.width * 4, 0);

            ++sliceIndex;
        }
    }

    stbi_image_free(self->base.pixels);
    self->base.pixels = nullptr;

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D10_1_SRV_DIMENSION_TEXTURE2DARRAY;
    srvDesc.Texture2DArray.MipLevels = (UINT)-1; // use the full mip chain
    srvDesc.Texture2DArray.ArraySize = sliceCount;
    srvDesc.Texture2DArray.FirstArraySlice = 0;
    srvDesc.Texture2DArray.MostDetailedMip = 0;

    hr = dxDevice->device->CreateShaderResourceView(self->texture, &srvDesc, &self->textureView);
    if (FAILED(hr) || !self->textureView)
        ReportHRError("Failed to create shader resource view", hr);
    else
        dxDevice->ctx->GenerateMips(self->textureView);

    tex->gpuLoaded = true;
}

static void DXBind(sgl_Texture* tex, uint32 unit)
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
    .Destroy = &DXDestroy,
    .Bind = &DXBind,
};

sgl_DXTexture2DArray* sgl_DXTexture2DArray_Create(void* data, sgl_Vec2i atlasSize, sgl_Vec2i frameSize)
{
    sgl_DXTexture2DArray* texture = sgl::Memory::New<sgl_DXTexture2DArray>();
    texture->base.base.size = atlasSize;
    texture->base.frameSize = frameSize;
    texture->base.base.vtable = &gDXVTable;
    texture->base.base.gpuLoaded = false;
    texture->base.pixels = data;

    uint32 cols = atlasSize.width / frameSize.width;
    uint32 rows = atlasSize.height / frameSize.height;
    texture->base.textureCount = cols * rows;

    texture->texture = nullptr;
    texture->textureView = nullptr;

    return texture;
}

#endif