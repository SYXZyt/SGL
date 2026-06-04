#include "DXTexture2D.h"
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

#define GetSelf sgl_DXTexture2D* self = (sgl_DXTexture2D*)tex

static void DXTexture_Destroy(sgl_Texture2D* tex)
{
    GetSelf;

    if (self->texture)
        self->texture->Release();

    sgl::Memory::Delete(self);
}

static void DXTexture_Bind(sgl_Texture2D* tex, uint32 unit)
{
    GetSelf;
    sgl_DXDevice* device = (sgl_DXDevice*)tex->gpu;

    device->ctx->PSSetShaderResources(unit, 1, &self->textureView);
}

static sgl_Texture2DVTable gDXVTable =
{
    .Destroy = &DXTexture_Destroy,
    .Bind = &DXTexture_Bind,
};

sgl_DXTexture2D* sgl_DXTexture2D_Create(sgl_GraphicsDevice* device, void* data, sgl_Vec2i size)
{
    sgl_DXTexture2D* texture = sgl::Memory::New<sgl_DXTexture2D>();
    texture->base.size = size;
    texture->base.vtable = &gDXVTable;

    D3D11_TEXTURE2D_DESC desc{};
    desc.Width = size.width;
    desc.Height = size.height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA initData{};
    initData.pSysMem = data;
    initData.SysMemPitch = size.width * 4;

    sgl_DXDevice* dxDevice = (sgl_DXDevice*)device;
    HRESULT hr = dxDevice->device->CreateTexture2D(
        &desc,
        &initData,
        &texture->texture);

    if (FAILED(hr))
    {
        ReportHRError("Failed to create texture", hr);
        return texture;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;

    hr = dxDevice->device->CreateShaderResourceView(
        texture->texture,
        &srvDesc,
        &texture->textureView);

    if (FAILED(hr))
    {
        ReportHRError("Failed to create shader resource view", hr);
        return texture;
    }

    dxDevice->ctx->GenerateMips(texture->textureView);

    return texture;
}