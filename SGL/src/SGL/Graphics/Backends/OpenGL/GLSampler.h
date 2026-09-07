#pragma once
#include <SGL/Graphics/Sampler.h>

SGL_BEGIN

typedef struct sgl_GLSampler sgl_sealed
{
    sgl_Sampler base;
    gluint samplerId;
} sgl_GLSampler;

SGL_API extern sgl_GLSampler* sgl_GLSampler_Create(sgl_TextureFilter filter, sgl_TextureClamp clamp);

SGL_END