#pragma once
#ifdef SGL_DIRECTX
#include <SGL/Graphics/GraphicsDevice.h>
#include <d3d11.h>

SGL_BEGIN

typedef struct sgl_DXDevice sgl_sealed
{
    sgl_GraphicsDevice base;

    ID3D11Device* device;
    ID3D11DeviceContext* ctx;
    IDXGISwapChain* swapchain;
    ID3D11SamplerState* sampler;

    ID3D11RenderTargetView* backBufferRtv;
} sgl_DXDevice;

SGL_API extern sgl_DXDevice* sgl_DXDevice_Create(sgl_Window* window);

SGL_END

#endif