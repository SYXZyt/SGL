#include "UniformBuffer.h"
#include <SGL/Util/Error.h>
#include <SGL/Graphics/Backends/OpenGL/GLUniformBuffer.h>
#include <SGL/Graphics/Backends/DirectX/DXUniformBuffer.h>

sgl_UniformBuffer* sgl_UniformBuffer_Create(sgl_GraphicsDevice* gpu, size_t size)
{
    sgl_UniformBuffer* buffer = nullptr;

    if (gpu->window->cfg.backend == sgl_Backend_OPENGL)
    {
        buffer = (sgl_UniformBuffer*)sgl_GLUniformBuffer_New(gpu, size);
    }
    else if (gpu->window->cfg.backend == sgl_Backend_DIRECTX11)
    {
#ifdef SGL_DIRECTX
        buffer = (sgl_UniformBuffer*)sgl_DXUniformBuffer_New(gpu, size);
#else
        SGL_REPORT_ERROR("DirectX is not supported on this platform");
        return 
#endif
    }
    else
    {
        SGL_REPORT_ERROR("Unsupported backend");
        return nullptr;
    }

    buffer->gpu = gpu;
    return buffer;
}

void sgl_UniformBuffer_Destroy(sgl_UniformBuffer* ub) {
    ub->vtable->Destroy(ub);
}

void sgl_UniformBuffer_Bind(sgl_UniformBuffer* ub, uint32 slot) {
    ub->vtable->Bind(ub, slot);
}

void sgl_UniformBuffer_Upload(sgl_UniformBuffer* ub, void* data) {
    ub->vtable->Upload(ub, data);
}