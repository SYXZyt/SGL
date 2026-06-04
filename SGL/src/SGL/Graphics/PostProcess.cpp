#include "PostProcess.h"
#include <SGL/Graphics/Backends/OpenGL/GLPostProcess.h>
#include <SGL/Graphics/Backends/DirectX/DXPostProcess.h>
#include <SGL/Util/Error.h>

sgl_PostProcess* sgl_PostProcess_Create(sgl_GraphicsDevice* device, sgl_Shader* shader)
{
    sgl_PostProcess* pp = nullptr;

    if (device->window->cfg.backend == sgl_Backend_OPENGL)
    {
        pp = (sgl_PostProcess*)sgl_GLPostProcess_Create(device, shader);
    }
    else if (device->window->cfg.backend == sgl_Backend_DIRECTX11)
    {
#ifdef SGL_DIRECTX
        pp = (sgl_PostProcess*)sgl_DXPostProcess_Create(device, shader);
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
    pp->shader = shader;
    return pp;
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