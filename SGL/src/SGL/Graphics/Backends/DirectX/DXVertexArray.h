#pragma once
#ifdef SGL_DIRECTX
#include <SGL/Graphics/VertexArray.h>
#include <SGL/Graphics/VertexLayout.h>
#include <d3d11.h>

SGL_BEGIN

typedef struct sgl_DXVertexArray sgl_sealed
{
    sgl_VertexArray base;
    ID3D11Buffer* vertexBuffer;

} sgl_DXVertexArray;

SGL_API extern sgl_DXVertexArray* sgl_DXVertexArray_New(uint32 vertexSize, sgl_VertexLayout* layout);

SGL_END

#endif