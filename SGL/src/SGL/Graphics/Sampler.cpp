#include "Sampler.h"
#include <SGL/Graphics/Backends/DirectX/DXSampler.h>
#include <SGL/Graphics/Backends/OpenGL/GLSampler.h>
#include <SGL/Util/Error.h>

sgl_Sampler* sgl_Sampler_Create(sgl_GraphicsDevice* gpu, sgl_TextureFilter filter, sgl_TextureClamp clamp)
{
    sgl_Sampler* sampler = nullptr;

    if (gpu->window->cfg.backend == sgl_Backend_OPENGL)
    {
        sampler = (sgl_Sampler*)sgl_GLSampler_Create(filter, clamp);
    }
    else if (gpu->window->cfg.backend == sgl_Backend_DIRECTX11)
    {
#ifdef SGL_DIRECTX
        sampler = (sgl_Sampler*)sgl_DXSampler_Create(filter, clamp);
#else
        SGL_REPORT_ERROR("DirectX is not supported on this platform");
        return nullptr;
#endif
    }
    else
    {
        SGL_REPORT_ERROR("Unsupported backend");
        return nullptr;
    }

    sampler->gpu = gpu;
    return sampler;
}

void sgl_Sampler_Destroy(sgl_Sampler* sampler) {
    sampler->vtable->Destroy(sampler);
}

void sgl_Sampler_Bind(sgl_Sampler* sampler, uint32 unit) {
    sampler->vtable->Bind(sampler, unit);
}