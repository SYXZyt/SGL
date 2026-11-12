#pragma once
#ifdef SGL_DIRECTX
#include <SGL/Graphics/Framebuffer.h>
#include <d3d11.h>

SGL_BEGIN

/// @brief DirectX offscreen render target
typedef struct sgl_DXFramebuffer sgl_sealed
{
    sgl_Framebuffer base;

    ID3D11Texture2D* texture;
    ID3D11RenderTargetView* rtv;
    ID3D11ShaderResourceView* srv;

    // Only valid when base.hasDepth
    ID3D11Texture2D* depthTexture;
    ID3D11DepthStencilView* dsv;
} sgl_DXFramebuffer;

/// @brief Create a DirectX offscreen render target
SGL_API extern sgl_DXFramebuffer* sgl_DXFramebuffer_Create(sgl_GraphicsDevice* device, sgl_Vec2i size, bool hasDepth, sgl_FramebufferFormat format);

SGL_END

#endif