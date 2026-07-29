#pragma once
#ifdef SGL_DIRECTX
#include <SGL/Graphics/UniformBuffer.h>
#include <d3d11.h>

SGL_BEGIN

/// @brief DirectX uniform buffer
typedef struct sgl_DXUniformBuffer sgl_sealed
{
    sgl_UniformBuffer base;
    ID3D11Buffer* buffer;
} sgl_DXUniformBuffer;

/// @brief Create a DirectX uniform buffer
SGL_API extern sgl_DXUniformBuffer* sgl_DXUniformBuffer_New(sgl_GraphicsDevice* gpu, size_t size);

SGL_END

#endif