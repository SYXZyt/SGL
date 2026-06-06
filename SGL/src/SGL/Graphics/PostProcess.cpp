#include "PostProcess.h"
#include <SGL/Graphics/Backends/OpenGL/GLPostProcess.h>
#include <SGL/Graphics/Backends/DirectX/DXPostProcess.h>
#include <SGL/Util/Error.h>
#include <SGL/Graphics/Shader.h>

sgl_PostProcess* sgl_PostProcess_Create(sgl_GraphicsDevice* device)
{
    sgl_PostProcess* pp = nullptr;

    if (device->window->cfg.backend == sgl_Backend_OPENGL)
    {
        pp = (sgl_PostProcess*)sgl_GLPostProcess_Create(device);
    }
    else if (device->window->cfg.backend == sgl_Backend_DIRECTX11)
    {
#ifdef SGL_DIRECTX
        pp = (sgl_PostProcess*)sgl_DXPostProcess_Create(device);
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

    pp->gpu = device;
    pp->shader = sgl_Shader_Create(device, device->screenQuadLayout);
    return pp;
}

sgl_Shader* sgl_PostProcess_GetShader(sgl_PostProcess* pp) {
    return pp->shader;
}

void sgl_PostProcess_Bind(sgl_PostProcess* pp) {
    pp->vtable->Bind(pp);
}

void sgl_PostProcess_OnResize(sgl_PostProcess* pp) {
    pp->vtable->OnResize(pp);
}

void sgl_PostProcess_Destroy(sgl_PostProcess* pp) {
    pp->vtable->Destroy(pp);
}