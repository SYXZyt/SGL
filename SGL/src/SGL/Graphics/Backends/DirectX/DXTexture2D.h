#pragma once
#ifdef SGL_DIRECTX
#include <SGL/Graphics/Texture2D.h>
#include <d3d11.h>

SGL_BEGIN

/// @brief DirectX texture
typedef struct sgl_DXTexture2D sgl_sealed
{
    sgl_Texture2D base;
    ID3D11Texture2D* texture;
    ID3D11ShaderResourceView* textureView;
    bool generateMipmaps;
} sgl_DXTexture2D;

/// @brief Create a new directx texture
SGL_API extern sgl_DXTexture2D* sgl_DXTexture2D_Create(void* data, sgl_Vec2i size, bool generateMipmaps);

SGL_END

#endif