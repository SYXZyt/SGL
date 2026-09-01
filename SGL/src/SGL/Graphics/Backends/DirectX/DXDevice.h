#pragma once
#ifdef SGL_DIRECTX
#include <SGL/Graphics/GraphicsDevice.h>
#include <d3d11.h>

SGL_BEGIN

/// @brief DirectX graphics device
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

    ID3D11Texture2D* sceneDepthTexture;
    ID3D11DepthStencilView* sceneDsv;
    ID3D11DepthStencilState* depthStencilState;
    ID3D11DepthStencilState* depthStencilDisabledState;
    ID3D11DepthStencilState* depthStencilReadOnlyState;

    ID3D11RasterizerState* rasterState;
    ID3D11RasterizerState* postProState;

    ID3D11BlendState* spriteBlendState;

    sgl_Shader* blitShader;

    ID3D11RenderTargetView* backBufferRtv;

    size_t boundTextureCount;
} sgl_DXDevice;

/// @brief Create a new DirectX device
SGL_API extern sgl_DXDevice* sgl_DXDevice_Create(sgl_Window* window, sgl_VertexLayout* screenQuadLayout);

SGL_END

#endif