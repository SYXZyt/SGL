#pragma once
#ifdef SGL_DIRECTX
#include <SGL/Graphics/PostProcess.h>
#include <d3d11.h>

SGL_BEGIN

/// @brief DirectX post processing effect
typedef struct sgl_DXPostProcess
{
    sgl_PostProcess base;
    ID3D11Texture2D* texture;
    ID3D11RenderTargetView* renderTargetView;
    ID3D11ShaderResourceView* shaderResourceView;
} sgl_DXPostProcess;

/// @brief Create a DirectX post processing effect
SGL_API extern sgl_DXPostProcess* sgl_DXPostProcess_Create(sgl_GraphicsDevice* device);

SGL_END

#endif