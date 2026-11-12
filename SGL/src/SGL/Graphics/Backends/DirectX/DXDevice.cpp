#ifdef SGL_DIRECTX
#include "DXDevice.h"
#include <SGL/Version.h>
#include <SGL/Util/Memory.h>
#include <SGL/Graphics/VertexArray.h>
#include <SGL/Graphics/Shader.h>
#include <SGL/Graphics/UniformBuffer.h>
#include <SGL/Graphics/Texture2D.h>
#include <SGL/Graphics/Framebuffer.h>
#include <SGL/Graphics/PostProcess.h>
#include <SGL/Graphics/Backends/DirectX/DXFramebuffer.h>
#include <SGL/Util/Logger.h>
#include <SGL/Util/Error.h>
#include <sstream>
#include <algorithm>
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
static void TryRelease(T*& t)
{
    if (t)
    {
        t->Release();
        t = nullptr;
    }
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

    int scale = dev->sceneScale < 1 ? 1 : dev->sceneScale;
    uint32 uscale = (uint32)scale;
    uint32 w = (dev->width + uscale - 1) / uscale;
    uint32 h = (dev->height + uscale - 1) / uscale;
    w += (w & 1);
    h += (h & 1);

    sgl_Framebuffer_Resize(dev->sceneFramebuffer, { (int)w, (int)h });

    D3D11_VIEWPORT viewport =
    {
        .Width = (FLOAT)dev->width,
        .Height = (FLOAT)dev->height,
        .MinDepth = 0.0f,
        .MaxDepth = 1.0f,
    };

    self->ctx->RSSetViewports(1, &viewport);

    D3D11_RECT defaultScissor = { 0, 0, (LONG)dev->width, (LONG)dev->height };
    self->ctx->RSSetScissorRects(1, &defaultScissor);
}

static void DXDevice_SetBlendMode(sgl_GraphicsDevice* dev, sgl_BlendMode mode)
{
    GetSelf;
    float blendFactor[4] = { 0, 0, 0, 0 };
    self->ctx->OMSetBlendState(self->blendStates[mode], blendFactor, 0xffffffff);
}

static void DXDevice_BeginFrame(sgl_GraphicsDevice* dev)
{
    GetSelf;

    sgl_Framebuffer_Bind(dev->sceneFramebuffer);
    sgl_Framebuffer_Clear(dev->sceneFramebuffer, dev->clearColour);

    dev->blendMode = sgl_BlendMode_ALPHA;
    float blendFactor[4] = { 0, 0, 0, 0 };
    self->ctx->OMSetBlendState(self->spriteBlendState, blendFactor, 0xffffffff);
}

static void DXDevice_BindBackbuffer(sgl_GraphicsDevice* dev)
{
    GetSelf;
    self->ctx->OMSetRenderTargets(1, &self->backBufferRtv, nullptr);
}

static void DXDevice_EndFrame(sgl_GraphicsDevice* dev)
{
    GetSelf;

    self->ctx->OMSetBlendState(nullptr, nullptr, 0xffffffff);
    self->ctx->RSSetState(self->postProState);

    D3D11_VIEWPORT fullViewport =
    {
        .Width = (FLOAT)dev->width,
        .Height = (FLOAT)dev->height,
        .MinDepth = 0.0f,
        .MaxDepth = 1.0f,
    };

    self->ctx->RSSetViewports(1, &fullViewport);

    sgl_PostProcess** effects;
    size_t effectCount = sgl_GraphicsDevice_GetEffects(dev, &effects);

    sgl_Texture* currentTexture = sgl_Framebuffer_GetTexture(dev->sceneFramebuffer);
    bool ranEffect = false;

    for (size_t i = 0; i < effectCount; ++i)
    {
        sgl_PostProcess* effect = effects[i];

        if (!sgl_PostProcess_GetEnabled(effect))
            continue;

        sgl_PostProcess_Bind(effect);
        sgl_Texture_Bind(currentTexture, 0);

        sgl_VertexArray_Bind(dev->screenQuad);

        self->ctx->Draw(dev->screenQuad->vertexCount, 0);

        ID3D11ShaderResourceView* nullSrv = nullptr;
        self->ctx->PSSetShaderResources(0, 1, &nullSrv);

        currentTexture = sgl_PostProcess_GetTexture(effect);
        ranEffect = true;
    }

    dev->vtable->BindBackbuffer(dev);

    if (!ranEffect)
    {
        sgl_Vec2i sceneSize = sgl_Framebuffer_GetSize(dev->sceneFramebuffer);
        int scale = dev->sceneScale < 1 ? 1 : dev->sceneScale;

        D3D11_VIEWPORT blitViewport =
        {
            .TopLeftX = ((FLOAT)dev->width - (FLOAT)sceneSize.width * scale) * 0.5f,
            .TopLeftY = ((FLOAT)dev->height - (FLOAT)sceneSize.height * scale) * 0.5f,
            .Width = (FLOAT)sceneSize.width * scale,
            .Height = (FLOAT)sceneSize.height * scale,
            .MinDepth = 0.0f,
            .MaxDepth = 1.0f,
        };

        self->ctx->RSSetViewports(1, &blitViewport);
    }

    sgl_Shader_Bind(self->blitShader);
    sgl_Texture_Bind(currentTexture, 0);
    sgl_VertexArray_Bind(dev->screenQuad);

    self->ctx->Draw(dev->screenQuad->vertexCount, 0);

    ID3D11ShaderResourceView* nullSrv = nullptr;
    self->ctx->PSSetShaderResources(0, 1, &nullSrv);
    self->ctx->RSSetState(self->rasterState);

    self->ctx->RSSetViewports(1, &fullViewport);

    dev->blendMode = sgl_BlendMode_ALPHA;
    float blendFactor[4] = { 0, 0, 0, 0 };
    self->ctx->OMSetBlendState(self->spriteBlendState, blendFactor, 0xffffffff);
}

static void DXDevice_SwapBuffer(sgl_GraphicsDevice* dev)
{
    GetSelf;
    self->swapchain->Present(dev->vsync, 0);
}

static void DXDevice_Destroy(sgl_GraphicsDevice* dev)
{
    GetSelf;

    sgl_Framebuffer_Destroy(dev->sceneFramebuffer);

    TryRelease(self->backBufferRtv);
    TryRelease(self->ctx);
    TryRelease(self->device);
    TryRelease(self->swapchain);
    TryRelease(self->depthStencilState);
    TryRelease(self->depthStencilDisabledState);
    TryRelease(self->depthStencilReadOnlyState);
    TryRelease(self->rasterState);
    TryRelease(self->postProState);
    for (ID3D11BlendState*& state : self->blendStates)
        TryRelease(state);
    self->spriteBlendState = nullptr;

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

// Vsync only requires sgl_GraphicsDevice::vsync which base sets. No calls needed unlike OpenGL
static void DXDevice_SetVSync(sgl_GraphicsDevice* dev, bool enable) {
}

static void DXDevice_SetScissor(sgl_GraphicsDevice* dev, bool enabled, sgl_Vec2i position, sgl_Vec2i size)
{
    GetSelf;

    D3D11_RECT rect{};

    if (enabled)
    {
        rect.left = position.x;
        rect.top = position.y;
        rect.right = position.x + size.width;
        rect.bottom = position.y + size.height;
    }
    else
    {
        rect.left = 0;
        rect.top = 0;
        rect.right = (LONG)dev->width;
        rect.bottom = (LONG)dev->height;
    }

    self->ctx->RSSetScissorRects(1, &rect);
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
    .SetVsync = &DXDevice_SetVSync,
    .SetScissor = &DXDevice_SetScissor,
    .BindBackbuffer = &DXDevice_BindBackbuffer,
    .SetBlendMode = &DXDevice_SetBlendMode,

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
    raster.ScissorEnable = TRUE;

    hr = device->device->CreateRasterizerState(&raster, &device->rasterState);
    if (FAILED(hr))
    {
        ReportHRError("Failed to create rasteriser state", hr);
        return device;
    }

    device->ctx->RSSetState(device->rasterState);

    D3D11_RECT defaultScissor = { 0, 0, (LONG)device->base.width, (LONG)device->base.height };
    device->ctx->RSSetScissorRects(1, &defaultScissor);

    raster.CullMode = D3D11_CULL_NONE;
    hr = device->device->CreateRasterizerState(&raster, &device->postProState);

#pragma region Framebuffer
    {
        device->base.sceneScale = 1;
        device->base.sceneFramebuffer = sgl_Framebuffer_Create((sgl_GraphicsDevice*)device, { (int)device->base.width, (int)device->base.height }, true);

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
    struct BlendSetup
    {
        bool enable;
        D3D11_BLEND src, dst;
        D3D11_BLEND_OP op;
        D3D11_BLEND srcAlpha, dstAlpha;
        D3D11_BLEND_OP opAlpha;
    };

    const BlendSetup blendSetups[sgl_BlendMode_COUNT] =
    {
        /* ALPHA    */ { true,  D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP_ADD, D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD },
        /* ADDITIVE */ { true,  D3D11_BLEND_ONE,       D3D11_BLEND_ONE,           D3D11_BLEND_OP_ADD, D3D11_BLEND_ONE, D3D11_BLEND_ONE,  D3D11_BLEND_OP_ADD },
        /* MULTIPLY */ { true,  D3D11_BLEND_DEST_COLOR, D3D11_BLEND_ZERO,         D3D11_BLEND_OP_ADD, D3D11_BLEND_DEST_ALPHA, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD },
        /* OPAQUE   */ { false, D3D11_BLEND_ONE,       D3D11_BLEND_ZERO,          D3D11_BLEND_OP_ADD, D3D11_BLEND_ONE, D3D11_BLEND_ZERO, D3D11_BLEND_OP_ADD },
        /* MAX      */ { true,  D3D11_BLEND_ONE,       D3D11_BLEND_ONE,           D3D11_BLEND_OP_MAX, D3D11_BLEND_ONE, D3D11_BLEND_ONE,  D3D11_BLEND_OP_MAX },
    };

    for (int i = 0; i < sgl_BlendMode_COUNT; ++i)
    {
        const BlendSetup& setup = blendSetups[i];

        D3D11_BLEND_DESC blendDesc{};
        blendDesc.RenderTarget[0].BlendEnable = setup.enable;
        blendDesc.RenderTarget[0].SrcBlend = setup.src;
        blendDesc.RenderTarget[0].DestBlend = setup.dst;
        blendDesc.RenderTarget[0].BlendOp = setup.op;
        blendDesc.RenderTarget[0].SrcBlendAlpha = setup.srcAlpha;
        blendDesc.RenderTarget[0].DestBlendAlpha = setup.dstAlpha;
        blendDesc.RenderTarget[0].BlendOpAlpha = setup.opAlpha;
        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        hr = device->device->CreateBlendState(&blendDesc, &device->blendStates[i]);
        if (FAILED(hr))
        {
            ReportHRError("Failed to create blend state", hr);
            return device;
        }
    }

    device->spriteBlendState = device->blendStates[sgl_BlendMode_ALPHA];
    device->base.blendMode = sgl_BlendMode_ALPHA;

    float blendFactor[4] = { 0, 0, 0, 0 };
    device->ctx->OMSetBlendState(device->spriteBlendState, blendFactor, 0xffffffff);

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