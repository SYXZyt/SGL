#ifdef SGL_DIRECTX
#include "DXDevice.h"
#include <SGL/Version.h>
#include <SGL/Util/Memory.h>
#include <SGL/Graphics/VertexArray.h>
#include <SGL/Graphics/Shader.h>
#include <SGL/Graphics/UniformBuffer.h>
#include <SGL/Graphics/Texture2D.h>
#include <SGL/Util/Logger.h>
#include <SGL/Util/Error.h>
#include <sstream>
#include <wrl/client.h>
#include <dxgi.h>
#include <imgui.h>
#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_sdl3.h>

#define GetSelf sgl_DXDevice* self = (sgl_DXDevice*)dev

static const char* FeatureLevelToString(D3D_FEATURE_LEVEL level)
{
    switch (level)
    {
        case D3D_FEATURE_LEVEL_11_0: return "11_0";
        case D3D_FEATURE_LEVEL_11_1: return "11_1";
        case D3D_FEATURE_LEVEL_12_0: return "12_0";
        case D3D_FEATURE_LEVEL_12_1: return "12_1";
        default: return "Unknown";
    }
}

static const char* VendorIdToString(uint32_t vendorId)
{
    switch (vendorId)
    {
        case 0x10DE: return "NVIDIA";
        case 0x1002: return "AMD";
        case 0x8086: return "Intel";
        default:     return "Unknown";
    }
}

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

    if (self->backBufferRtv)
        self->backBufferRtv->Release();

    HRESULT hr = self->swapchain->ResizeBuffers(0, dev->width, dev->height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
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
    TryRelease(self->sampler);

    sgl::Memory::Delete(self);
}

static void DXDevice_Draw(sgl_GraphicsDevice* dev, sgl_VertexArray* va, sgl_Shader* shr, sgl_Texture** textures, size_t textureCount, sgl_UniformBuffer** buffers, size_t bufferCount)
{
    GetSelf;

    sgl_Shader_Bind(shr);
    sgl_VertexArray_Bind(va);

    for (size_t i = 0; i < textureCount; ++i)
        sgl_Texture_Bind(textures[i], (uint32)i);

    for (size_t i = 0; i < bufferCount; ++i)
        sgl_UniformBuffer_Bind(buffers[i], (uint32)i);

    self->ctx->Draw(va->vertexCount, 0);
}

static void DXDevice_ImGui_Init(sgl_GraphicsDevice* dev)
{
    GetSelf;

    if (dev->window->cfg.enableImGui)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGui_ImplSDL3_InitForD3D(dev->window->window);
        ImGui_ImplDX11_Init(self->device, self->ctx);
    }
}

static void DXDevice_ImGui_Shutdown(sgl_GraphicsDevice* dev)
{
    if (dev->window->cfg.enableImGui)
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }
}

static void DXDevice_ImGui_NewFrame(sgl_GraphicsDevice* dev)
{
    if (dev->window->cfg.enableImGui)
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
    }
}

static void DXDevice_ImGui_RenderDrawData(sgl_GraphicsDevice* dev)
{
    if (dev->window->cfg.enableImGui)
    {
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
}

static const sgl_GraphicsDeviceVTable gDxVTable =
{
    .SetClearColour = &sgl_GraphicsDevice_SetClearColour,
    .Resize = &DXDevice_Resize,
    .BeginFrame = &DXDevice_Clear,
    .EndFrame = &DXDevice_Present,
    .Destroy = &DXDevice_Destroy,
    .Draw = &DXDevice_Draw,

    .ImGui_Init = &DXDevice_ImGui_Init,
    .ImGui_Shutdown = &DXDevice_ImGui_Shutdown,
    .ImGui_NewFrame = &DXDevice_ImGui_NewFrame,
    .ImGui_RenderDrawData = &DXDevice_ImGui_RenderDrawData,
};


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
    swapDesc.OutputWindow = sgl_Window_GetWin32Window(window);
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
    raster.FrontCounterClockwise = true;

    ID3D11RasterizerState* rasterState = nullptr;

    hr = device->device->CreateRasterizerState(&raster, &rasterState);
    if (FAILED(hr))
    {
        ReportHRError("Failed to create rasteriser state", hr);
        return device;
    }

    device->ctx->RSSetState(rasterState);
    rasterState->Release();

#pragma region Blend State
    D3D11_BLEND_DESC blendDesc{};
    blendDesc.RenderTarget[0].BlendEnable = true;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    ID3D11BlendState* blendState = nullptr;
    hr = device->device->CreateBlendState(&blendDesc, &blendState);
    if (FAILED(hr))
    {
        ReportHRError("Failed to create blend state", hr);
        return device;
    }

    float blendFactor[4] = { 0, 0, 0, 0 };
    device->ctx->OMSetBlendState(blendState, blendFactor, 0xffffffff);
    blendState->Release();

#pragma endregion

#pragma region Sampler State
    D3D11_SAMPLER_DESC sampDesc{};
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    hr = device->device->CreateSamplerState(&sampDesc, &device->sampler);
    if (FAILED(hr))
    {
        ReportHRError("Failed to create sampler state", hr);
        return device;
    }

    device->ctx->PSSetSamplers(0, 1, &device->sampler);
#pragma endregion

    std::stringstream ss;

    ss << "SDL Version: " << SDL_VERSIONNUM_MAJOR(SDL_VERSION) << "." << SDL_VERSIONNUM_MINOR(SDL_VERSION) << "." << SDL_VERSIONNUM_MICRO(SDL_VERSION);
    sgl_Log(ss.str().c_str());
    ss.str("");

    ss << "SGL Version: " << sgl_VersionString();
    sgl_Log(ss.str().c_str());
    ss.str("");

    D3D_FEATURE_LEVEL featureLevel = device->device->GetFeatureLevel();
    ss << "D3D Feature Level: " << FeatureLevelToString(featureLevel);
    sgl_Log(ss.str().c_str());
    ss.str("");

    Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
    Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
    DXGI_ADAPTER_DESC desc;
    char gpuName[128] = {};
    const char* vendorName;
    uint64_t vramMB;

    if (FAILED(device->device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice)))
        goto ret;

    if (FAILED(dxgiDevice->GetAdapter(&adapter)))
        goto ret;

    if (FAILED(adapter->GetDesc(&desc)))
        goto ret;

    std::wcstombs(gpuName, desc.Description, sizeof(gpuName) - 1);

    vendorName = VendorIdToString(desc.VendorId);

    ss << "Vendor: " << vendorName << " (0x"
        << std::hex << desc.VendorId << std::dec << ")";
    sgl_Log(ss.str().c_str());
    ss.str("");

    ss << "Renderer: " << gpuName;
    sgl_Log(ss.str().c_str());
    ss.str("");

    vramMB = desc.DedicatedVideoMemory / (1024ull * 1024ull);
    ss << "Dedicated VRAM: " << vramMB << " MB";
    sgl_Log(ss.str().c_str());

ret:
    return device;
}

#endif