#pragma once
#ifdef SGL_DIRECTX
#include <SGL/Graphics/Sampler.h>
#include <d3d11.h>

SGL_BEGIN

typedef struct sgl_DXSampler sgl_sealed
{
    sgl_Sampler base;

    ID3D11SamplerState* samplerstate;
} sgl_DXSampler;

SGL_API extern sgl_DXSampler* sgl_DXSampler_Create(sgl_TextureFilter filter, sgl_TextureClamp clamp);

SGL_END
#endif