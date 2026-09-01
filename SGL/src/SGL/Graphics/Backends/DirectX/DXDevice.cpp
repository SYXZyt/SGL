#ifdef SGL_DIRECTX
#include "DXDevice.h"
#include <SGL/Version.h>
#include <SGL/Util/Memory.h>
#include <SGL/Graphics/VertexArray.h>
#include <SGL/Graphics/Shader.h>
#include <SGL/Graphics/UniformBuffer.h>
#include <SGL/Graphics/Texture2D.h>
#include <SGL/Graphics/Backends/DirectX/DXPostProcess.h>
#include <SGL/Util/Logger.h>
#include <SGL/Util/Error.h>
#include <sstream>
#include <wrl/client.h>
#include <dxgi.h>
#include <imgui.h>
#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_sdl3.h>

#define GetSelf sgl_DXDevice* self = (sgl_DXDevice*)dev

const char* BlitVertex =
"struct VSInput\n"
"{\n"
"    float3 Pos : POSITION;\n"
"    float2 UV : TEXCOORD0;\n"
"};\n"
"\n"
"struct PSInput\n"
"{\n"
"    float4 Pos : SV_POSITION;\n"
"    float2 UV : TEXCOORD0;\n"
"};\n"
"\n"
"PSInput main(VSInput input)\n"
"{\n"
"    PSInput output;\n"
"    output.Pos = float4(input.Pos, 1.0f);\n"
"    output.UV = input.UV;\n"
"    return output;\n"
"}\n";

const char* BlitFragment =
"Texture2D FrameTexture : register(t0);\n"
"SamplerState FrameSampler : register(s0);\n"
"\n"
"struct PSInput\n"
"{\n"
"    float4 Pos : SV_POSITION;\n"
"    float2 UV : TEXCOORD0;\n"
"};\n"
"\n"
"float4 main(PSInput input) : SV_TARGET\n"
"{\n"
"    float4 colour = FrameTexture.Sample(FrameSampler, input.UV);\n"
"    return colour;\n"
"}\n";

template <typename T>
static void TryRelease(T* t)
{
    if (t)
        t->Release();
}

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

static void DXDevice_ApplyDepthStencilState(sgl_DXDevice* self)
{
    ID3D11DepthStencilState* state;

    if (!self->base.depthTestEnabled)
        state = self->depthStencilDisabledState;
    else if (self->base.depthWriteEnabled)
        state = self->depthStencilState;
    else
        state = self->depthStencilReadOnlyState;

    self->ctx->OMSetDepthStencilState(state, 0);
}

static void DXDevice_SetDepthTestEnabled(sgl_GraphicsDevice* dev, bool enabled)
{
    GetSelf;
    DXDevice_ApplyDepthStencilState(self);
}

static void DXDevice_SetDepthWriteEnabled(sgl_GraphicsDevice* dev, bool enabled)
{
    GetSelf;
    DXDevice_ApplyDepthStencilState(self);
}

static void DXDevice_Resize(sgl_GraphicsDevice* dev, sgl_Vec2i newSize)
{
    GetSelf;

    dev->width = newSize.width;
    dev->height = newSize.height;

    self->ctx->OMSetRenderTargets(0, nullptr, nullptr);

    TryRelease(self->backBufferRtv);

    TryRelease(self->sceneSrv);
    TryRelease(self->sceneRtv);
    TryRelease(self->sceneTexture);

    TryRelease(self->sceneDsv);
    TryRelease(self->sceneDepthTexture);

    HRESULT hr = self->swapchain->ResizeBuffers(
        0,
        dev->width,
        dev->height,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        0
    );

    if (FAILED(hr))
    {
        ReportHRError("Failed to resize swapchain", hr);
        return;
    }

    RecreateBackbuffer(self);

    D3D11_TEXTURE2D_DESC sceneDesc = {};
    sceneDesc.Width = dev->width;
    sceneDesc.Height = dev->height;
    sceneDesc.MipLevels = 1;
    sceneDesc.ArraySize = 1;
    sceneDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sceneDesc.SampleDesc.Count = 1;
    sceneDesc.Usage = D3D11_USAGE_DEFAULT;
    sceneDesc.BindFlags =
        D3D11_BIND_RENDER_TARGET |
        D3D11_BIND_SHADER_RESOURCE;

    hr = self->device->CreateTexture2D(
        &sceneDesc,
        nullptr,
        &self->sceneTexture
    );

    if (FAILED(hr))
    {
        ReportHRError("Failed to create scene texture", hr);
        return;
    }

    hr = self->device->CreateRenderTargetView(
        self->sceneTexture,
        nullptr,
        &self->sceneRtv
    );

    if (FAILED(hr))
    {
        ReportHRError("Failed to create scene RTV", hr);
        return;
    }

    hr = self->device->CreateShaderResourceView(
        self->sceneTexture,
        nullptr,
        &self->sceneSrv
    );

    if (FAILED(hr))
    {
        ReportHRError("Failed to create scene SRV", hr);
        return;
    }

    D3D11_TEXTURE2D_DESC depthDesc = {};
    depthDesc.Width = dev->width;
    depthDesc.Height = dev->height;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc.SampleDesc.Count = 1;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    hr = self->device->CreateTexture2D(&depthDesc, nullptr, &self->sceneDepthTexture);
    if (FAILED(hr))
    {
        ReportHRError("Failed to create scene depth texture", hr);
        return;
    }

    hr = self->device->CreateDepthStencilView(self->sceneDepthTexture, nullptr, &self->sceneDsv);
    if (FAILED(hr))
    {
        ReportHRError("Failed to create scene DSV", hr);
        return;
    }

    D3D11_VIEWPORT viewport =
    {
        .Width = (FLOAT)dev->width,
        .Height = (FLOAT)dev->height,
        .MinDepth = 0.0f,
        .MaxDepth = 1.0f,
    };

    self->ctx->RSSetViewports(1, &viewport);
}

static void DXDevice_BeginFrame(sgl_GraphicsDevice* dev)
{
    GetSelf;

    self->ctx->OMSetRenderTargets(1, &self->sceneRtv, self->sceneDsv);

    self->ctx->ClearRenderTargetView(
        self->sceneRtv,
        &dev->clearColour.r
    );

    self->ctx->ClearDepthStencilView(self->sceneDsv, D3D11_CLEAR_DEPTH, 1.0f, 0);

    float blendFactor[4] = { 0, 0, 0, 0 };
    self->ctx->OMSetBlendState(self->spriteBlendState, blendFactor, 0xffffffff);
}

static void DXDevice_EndFrame(sgl_GraphicsDevice* dev)
{
    GetSelf;

    self->ctx->OMSetBlendState(nullptr, nullptr, 0xffffffff);

    self->ctx->RSSetState(self->postProState);

    sgl_PostProcess** effects;
    size_t effectCount = sgl_GraphicsDevice_GetEffects(dev, &effects);

    ID3D11ShaderResourceView* currentSrv = self->sceneSrv;

    self->ctx->PSSetSamplers(0, 1, &self->postProSampler);

    for (size_t i = 0; i < effectCount; ++i)
    {
        sgl_DXPostProcess* effect = (sgl_DXPostProcess*)effects[i];

        if (!effect->base.enabled)
            continue;

        self->ctx->OMSetRenderTargets(1, &effect->renderTargetView, nullptr);

        sgl_PostProcess_Bind(effects[i]);

        self->ctx->PSSetShaderResources(0, 1, &currentSrv);

        sgl_VertexArray_Bind(dev->screenQuad);

        self->ctx->Draw(dev->screenQuad->vertexCount, 0);

        ID3D11ShaderResourceView* nullSrv = nullptr;
        self->ctx->PSSetShaderResources(0, 1, &nullSrv);

        currentSrv = effect->shaderResourceView;
    }

    self->ctx->PSSetSamplers(0, 1, &self->sampler);

    self->ctx->OMSetRenderTargets(1, &self->backBufferRtv, nullptr);
    sgl_Shader_Bind(self->blitShader);
    self->ctx->PSSetShaderResources(0, 1, &currentSrv);
    sgl_VertexArray_Bind(dev->screenQuad);

    self->ctx->Draw(dev->screenQuad->vertexCount, 0);

    ID3D11ShaderResourceView* nullSrv = nullptr;
    self->ctx->PSSetShaderResources(0, 1, &nullSrv);

    self->ctx->RSSetState(self->rasterState);
}

static void DXDevice_SwapBuffer(sgl_GraphicsDevice* dev)
{
    GetSelf;
    self->swapchain->Present(1, 0);
}

static void DXDevice_Destroy(sgl_GraphicsDevice* dev)
{
    GetSelf;

    TryRelease(self->backBufferRtv);
    TryRelease(self->ctx);
    TryRelease(self->device);
    TryRelease(self->swapchain);
    TryRelease(self->sampler);
    TryRelease(self->postProSampler);
    TryRelease(self->sceneRtv);
    TryRelease(self->sceneSrv);
    TryRelease(self->sceneTexture);
    TryRelease(self->sceneDsv);
    TryRelease(self->sceneDepthTexture);
    TryRelease(self->depthStencilState);
    TryRelease(self->depthStencilDisabledState);
    TryRelease(self->depthStencilReadOnlyState);
    TryRelease(self->rasterState);
    TryRelease(self->postProState);
    TryRelease(self->spriteBlendState);

    sgl_Shader_Destroy(self->blitShader);

    sgl::Memory::Delete(self);
}

static void DXDevice_DrawInstanced(sgl_GraphicsDevice* dev, sgl_VertexArray* va, sgl_Shader* shr, sgl_Texture** textures, size_t textureCount, sgl_UniformBuffer** buffers, size_t bufferCount, uint32 instanceCount)
{
    GetSelf;

    sgl_Shader_Bind(shr);
    sgl_VertexArray_Bind(va);

    for (size_t i = 0; i < textureCount; ++i)
        sgl_Texture_Bind(textures[i], (uint32)i);

    if (self->boundTextureCount > textureCount)
    {
        static ID3D11ShaderResourceView* const nullSrvs[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = {};
        size_t staleCount = self->boundTextureCount - textureCount;
        self->ctx->PSSetShaderResources((UINT)textureCount, (UINT)staleCount, nullSrvs);
    }

    self->boundTextureCount = textureCount;

    for (size_t i = 0; i < bufferCount; ++i)
        sgl_UniformBuffer_Bind(buffers[i], (uint32)i);

    if (va->indexCount > 0)
        self->ctx->DrawIndexedInstanced(va->indexCount, instanceCount, 0, 0, 0);
    else
        self->ctx->DrawInstanced(va->vertexCount, instanceCount, 0, 0);
}

static void DXDevice_Draw(sgl_GraphicsDevice* dev, sgl_VertexArray* va, sgl_Shader* shr, sgl_Texture** textures, size_t textureCount, sgl_UniformBuffer** buffers, size_t bufferCount)
{
    GetSelf;

    sgl_Shader_Bind(shr);
    sgl_VertexArray_Bind(va);

    for (size_t i = 0; i < textureCount; ++i)
        sgl_Texture_Bind(textures[i], (uint32)i);

    if (self->boundTextureCount > textureCount)
    {
        static ID3D11ShaderResourceView* const nullSrvs[D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNT] = {};
        size_t staleCount = self->boundTextureCount - textureCount;
        self->ctx->PSSetShaderResources((UINT)textureCount, (UINT)staleCount, nullSrvs);
    }

    self->boundTextureCount = textureCount;

    for (size_t i = 0; i < bufferCount; ++i)
        sgl_UniformBuffer_Bind(buffers[i], (uint32)i);

    if (va->indexCount > 0)
        self->ctx->DrawIndexed(va->indexCount, 0, 0);
    else
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
    .SetClearColour = &DXDevice_SetClearColour,
    .SetDepthTestEnabled = &DXDevice_SetDepthTestEnabled,
    .SetDepthWriteEnabled = &DXDevice_SetDepthWriteEnabled,
    .Resize = &DXDevice_Resize,
    .BeginFrame = &DXDevice_BeginFrame,
    .EndFrame = &DXDevice_EndFrame,
    .Destroy = &DXDevice_Destroy,
    .SwapBuffer = &DXDevice_SwapBuffer,
    .Draw = &DXDevice_Draw,
    .DrawInstanced = &DXDevice_DrawInstanced,

    .ImGui_Init = &DXDevice_ImGui_Init,
    .ImGui_Shutdown = &DXDevice_ImGui_Shutdown,
    .ImGui_NewFrame = &DXDevice_ImGui_NewFrame,
    .ImGui_RenderDrawData = &DXDevice_ImGui_RenderDrawData,
};


sgl_DXDevice* sgl_DXDevice_Create(sgl_Window* window, sgl_VertexLayout* screenQuadLayout)
{
    sgl_DXDevice* device = sgl::Memory::New<sgl_DXDevice>();

    UINT deviceFlags = 0;
#ifdef _DEBUG
    deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    device->base.clearColour = sgl_Col_CornflowerBlue;
    device->base.window = window;
    device->base.width = window->screenSize.width;
    device->base.height = window->screenSize.height;
    device->base.vtable = &gDxVTable;
    device->boundTextureCount = 0;

    DXGI_SWAP_CHAIN_DESC swapDesc = {};
    swapDesc.BufferCount = 1;
    swapDesc.BufferDesc.Width = device->base.width;
    swapDesc.BufferDesc.Height = device->base.height;
    swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.OutputWindow = sgl_Window_GetWin32Window(window);
    swapDesc.SampleDesc.Count = 1;
    swapDesc.Windowed = TRUE;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlags, nullptr, 0, D3D11_SDK_VERSION, &swapDesc, &device->swapchain, &device->device, nullptr, &device->ctx);
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

    hr = device->device->CreateRasterizerState(&raster, &device->rasterState);
    if (FAILED(hr))
    {
        ReportHRError("Failed to create rasteriser state", hr);
        return device;
    }

    device->ctx->RSSetState(device->rasterState);

    raster.CullMode = D3D11_CULL_NONE;
    hr = device->device->CreateRasterizerState(&raster, &device->postProState);

#pragma region Framebuffer
    {
        D3D11_TEXTURE2D_DESC desc = {};
        desc.Width = device->base.width;
        desc.Height = device->base.height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags =
            D3D11_BIND_RENDER_TARGET |
            D3D11_BIND_SHADER_RESOURCE;

        device->device->CreateTexture2D(
            &desc,
            nullptr,
            &device->sceneTexture
        );

        device->device->CreateRenderTargetView(
            device->sceneTexture,
            nullptr,
            &device->sceneRtv
        );

        device->device->CreateShaderResourceView(
            device->sceneTexture,
            nullptr,
            &device->sceneSrv
        );

        D3D11_TEXTURE2D_DESC depthDesc = {};
        depthDesc.Width = device->base.width;
        depthDesc.Height = device->base.height;
        depthDesc.MipLevels = 1;
        depthDesc.ArraySize = 1;
        depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthDesc.SampleDesc.Count = 1;
        depthDesc.Usage = D3D11_USAGE_DEFAULT;
        depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        hr = device->device->CreateTexture2D(&depthDesc, nullptr, &device->sceneDepthTexture);
        if (FAILED(hr))
        {
            ReportHRError("Failed to create scene depth texture", hr);
            return device;
        }

        hr = device->device->CreateDepthStencilView(device->sceneDepthTexture, nullptr, &device->sceneDsv);
        if (FAILED(hr))
        {
            ReportHRError("Failed to create scene DSV", hr);
            return device;
        }

        D3D11_DEPTH_STENCIL_DESC depthStateDesc = {};
        depthStateDesc.DepthEnable = true;
        depthStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depthStateDesc.DepthFunc = D3D11_COMPARISON_LESS;

        hr = device->device->CreateDepthStencilState(&depthStateDesc, &device->depthStencilState);
        if (FAILED(hr))
        {
            ReportHRError("Failed to create depth-stencil state", hr);
            return device;
        }

        D3D11_DEPTH_STENCIL_DESC depthDisabledDesc = {};
        depthDisabledDesc.DepthEnable = false;
        depthDisabledDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        depthDisabledDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;

        hr = device->device->CreateDepthStencilState(&depthDisabledDesc, &device->depthStencilDisabledState);
        if (FAILED(hr))
        {
            ReportHRError("Failed to create disabled depth-stencil state", hr);
            return device;
        }

        D3D11_DEPTH_STENCIL_DESC depthReadOnlyDesc = {};
        depthReadOnlyDesc.DepthEnable = true;
        depthReadOnlyDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        depthReadOnlyDesc.DepthFunc = D3D11_COMPARISON_LESS;

        hr = device->device->CreateDepthStencilState(&depthReadOnlyDesc, &device->depthStencilReadOnlyState);
        if (FAILED(hr))
        {
            ReportHRError("Failed to create read-only depth-stencil state", hr);
            return device;
        }

        device->ctx->OMSetDepthStencilState(device->depthStencilState, 0);
    }
#pragma endregion

#pragma region Blend State
    D3D11_BLEND_DESC blendDesc{};
    blendDesc.RenderTarget[0].BlendEnable = true;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    hr = device->device->CreateBlendState(&blendDesc, &device->spriteBlendState);
    if (FAILED(hr))
    {
        ReportHRError("Failed to create blend state", hr);
        return device;
    }

    float blendFactor[4] = { 0, 0, 0, 0 };
    device->ctx->OMSetBlendState(device->spriteBlendState, blendFactor, 0xffffffff);

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

    D3D11_SAMPLER_DESC postProSampDesc = sampDesc;
    postProSampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

    hr = device->device->CreateSamplerState(&postProSampDesc, &device->postProSampler);
    if (FAILED(hr))
    {
        ReportHRError("Failed to create post-process sampler state", hr);
        return device;
    }
#pragma endregion

    device->blitShader = sgl_Shader_Create((sgl_GraphicsDevice*)device, screenQuadLayout);
    sgl_Shader_Load_Source(device->blitShader, BlitVertex, BlitFragment);

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