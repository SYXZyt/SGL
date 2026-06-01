#ifdef SGL_DIRECTX
#include "DXDevice.h"
#include <SGL/Util/Memory.h>
#include <SGL/Graphics/VertexArray.h>
#include <SGL/Graphics/Shader.h>
#include <SGL/Util/Logger.h>
#include <SGL/Util/Error.h>

#include <dxgi.h>
#include <sstream>

#define GetSelf sgl_DXDevice* self = (sgl_DXDevice*)dev

static void ReportHRError(const char* message, HRESULT hr)
{
    std::stringstream ss;
    ss << message << " (" << sgl_HResultToString(hr) << ')';
    SGL_REPORT_ERROR(ss.str().c_str());
}

static void RecreateBackbuffer(sgl_DXDevice* self)
{
    if (self->backBufferRtv)
    {
        self->backBufferRtv->Release();
        self->backBufferRtv = nullptr;
    }

    ID3D11Texture2D* backBuffer = nullptr;

    HRESULT hr = self->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    if (FAILED(hr))
    {
        ReportHRError("Failed to get buffer", hr);
        return;
    }
    

    hr = self->device->CreateRenderTargetView(backBuffer, nullptr, &self->backBufferRtv);
    if (FAILED(hr))
    {
        ReportHRError("Failed to create render target view", hr);
        return;
    }

    backBuffer->Release();

    self->ctx->OMSetRenderTargets(1, &self->backBufferRtv, nullptr);
}

static void DXDevice_SetClearColour(sgl_GraphicsDevice* dev, sgl_Colour colour) {
    dev->clearColour = colour;
}

static void DXDevice_Resize(sgl_GraphicsDevice* dev, sgl_Vec2i newSize)
{
    GetSelf;

    dev->width = newSize.width;
    dev->height = newSize.height;

    self->ctx->OMSetRenderTargets(0, nullptr, nullptr);

    HRESULT hr = self->swapchain->ResizeBuffers(0, dev->width, dev->height, DXGI_FORMAT_UNKNOWN, 0);
    if (FAILED(hr))
    {
        ReportHRError("Failed to resize swapchain", hr);
        return;
    }

    RecreateBackbuffer(self);

    D3D11_VIEWPORT viewport =
    {
        .Width = (FLOAT)dev->width,
        .Height = (FLOAT)dev->height,
        .MinDepth = 0.f,
        .MaxDepth = 1.f,
    };

    self->ctx->RSSetViewports(1, &viewport);
}

static void DXDevice_Clear(sgl_GraphicsDevice* dev)
{
    GetSelf;
    self->ctx->ClearRenderTargetView(self->backBufferRtv, &dev->clearColour.r);
}

static void DXDevice_Present(sgl_GraphicsDevice* dev)
{
    GetSelf;
    self->swapchain->Present(1, 0);
}

template <typename T>
static void TryRelease(T* t)
{
    if (t)
        t->Release();
}

static void DXDevice_Destroy(sgl_GraphicsDevice* dev)
{
    GetSelf;

    TryRelease(self->backBufferRtv);
    TryRelease(self->ctx);
    TryRelease(self->device);
    TryRelease(self->swapchain);

    sgl::Memory::Delete(self);
}

static void DXDevice_Draw(sgl_GraphicsDevice* dev, sgl_VertexArray* va, sgl_Shader* shr)
{
    GetSelf;

    sgl_Shader_Bind(shr);
    sgl_VertexArray_Bind(va);

    self->ctx->Draw(va->vertexCount, 0);
}

static const sgl_GraphicsDeviceVTable gDxVTable =
{
    .SetClearColour = &sgl_GraphicsDevice_SetClearColour,
    .Resize = &DXDevice_Resize,
    .Clear = &DXDevice_Clear,
    .Present = &DXDevice_Present,
    .Destroy = &DXDevice_Destroy,
    .Draw = &DXDevice_Draw,
};

#endif

sgl_DXDevice* sgl_DXDevice_Create(sgl_Window* window)
{
    sgl_DXDevice* device = sgl::Memory::New<sgl_DXDevice>();

    device->base.clearColour = sgl_Col_CornflowerBlue;
    device->base.window = window;
    device->base.width = window->screenSize.width;
    device->base.height = window->screenSize.height;
    device->base.vtable = &gDxVTable;

    DXGI_SWAP_CHAIN_DESC swapDesc = {};
    swapDesc.BufferCount = 1;
    swapDesc.BufferDesc.Width = device->base.width;
    swapDesc.BufferDesc.Height = device->base.height;
    swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.OutputWindow = NULL; /// @todo sgl_Window_Native(window);
    swapDesc.SampleDesc.Count = 1;
    swapDesc.Windowed = TRUE;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &swapDesc, &device->swapchain, &device->device, nullptr, &device->ctx);
    if (FAILED(hr))
    {
        ReportHRError("Failed to create device and swapchain", hr);
        return device;
    }

    RecreateBackbuffer(device);

    D3D11_VIEWPORT viewport =
    {
        .Width = (FLOAT)window->screenSize.width,
        .Height = (FLOAT)window->screenSize.height,
        .MinDepth = 0.f,
        .MaxDepth = 1.f,
    };

    device->ctx->RSSetViewports(1, &viewport);

    D3D11_RASTERIZER_DESC raster = {};
    raster.FillMode = D3D11_FILL_SOLID;
    raster.CullMode = D3D11_CULL_BACK;
    raster.FrontCounterClockwise = false;

    ID3D11RasterizerState* rasterState = nullptr;

    hr = device->device->CreateRasterizerState(&raster, &rasterState);
    if (FAILED(hr))
    {
        ReportHRError("Failed to create rasteriser state", hr);
        return device;
    }

    device->ctx->RSSetState(rasterState);
    rasterState->Release();

    return device;
}