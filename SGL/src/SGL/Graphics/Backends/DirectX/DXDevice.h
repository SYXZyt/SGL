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

    ID3D11Texture2D* sceneTexture;
    ID3D11RenderTargetView* sceneRtv;
    ID3D11ShaderResourceView* sceneSrv;

    ID3D11RasterizerState* rasterState;
    ID3D11RasterizerState* postProState;

    sgl_Shader* blitShader;

    ID3D11RenderTargetView* backBufferRtv;
} sgl_DXDevice;

SGL_API extern sgl_DXDevice* sgl_DXDevice_Create(sgl_Window* window, sgl_VertexLayout* screenQuadLayout);

SGL_END

#endif