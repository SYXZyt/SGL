#pragma once
#ifdef SGL_DIRECTX
#include <SGL/Graphics/Shader.h>
#include <d3d11.h>
#include <SGL/Graphics/VertexLayout.h>

SGL_BEGIN

typedef struct sgl_DXShader sgl_sealed
{
    sgl_Shader base;

    ID3D11VertexShader* vertexShader;
    ID3D11PixelShader* pixelShader;
    ID3D11InputLayout* inputLayout;

    ID3DBlob* vertexBlob;
    ID3DBlob* pixelBlob;
} sgl_DXShader;

SGL_API extern sgl_DXShader* sgl_DXShader_Create(const char* vsrc, const char* fsrc, sgl_VertexLayout* vertexLayout);

SGL_END

#endif