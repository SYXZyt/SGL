#pragma once
#ifdef SGL_DIRECTX
#include <SGL/Graphics/Texture2DArray.h>
#include <SGL/Graphics/GraphicsDevice.h>
#include <d3d11.h>

SGL_BEGIN

typedef struct sgl_DXTexture2DArray sgl_sealed
{
    sgl_Texture2DArray base;
    ID3D11Texture2D* texture;
    ID3D11ShaderResourceView* textureView;
} sgl_DXTexture2DArray;

SGL_API extern sgl_DXTexture2DArray* sgl_DXTexture2DArray_Create(sgl_GraphicsDevice* device, void* data, sgl_Vec2i atlasSize, sgl_Vec2i frameSize);

SGL_END

#endif