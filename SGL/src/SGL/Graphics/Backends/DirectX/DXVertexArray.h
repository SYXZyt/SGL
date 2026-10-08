#pragma once
#ifdef SGL_DIRECTX
#include <SGL/Graphics/VertexArray.h>
#include <SGL/Graphics/VertexLayout.h>
#include <d3d11.h>

SGL_BEGIN

/// @brief DirectX vertex array
typedef struct sgl_DXVertexArray sgl_sealed
{
    sgl_VertexArray base;
    ID3D11Buffer* vertexBuffer;
    ID3D11Buffer* indexBuffer;
    ID3D11Buffer* instanceBuffer;

} sgl_DXVertexArray;

/// @brief Create a DirectX vertex array
SGL_API extern sgl_DXVertexArray* sgl_DXVertexArray_New(uint32 vertexSize, sgl_VertexLayout* layout, uint32 instanceSize);

SGL_END

#endif