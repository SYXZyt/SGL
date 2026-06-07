#include "PostProcess.h"
#include <SGL/Graphics/Backends/OpenGL/GLPostProcess.h>
#include <SGL/Graphics/Backends/DirectX/DXPostProcess.h>
#include <SGL/Util/Error.h>
#include <SGL/Graphics/Shader.h>
#include <SGL/Graphics/UniformBuffer.h>
#include <SGL/Util/Memory.h>

sgl_PostProcess* sgl_PostProcess_Create(sgl_GraphicsDevice* device, size_t uniformCount)
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

    pp->uniforms = (sgl_UniformBuffer**)sgl_Malloc(uniformCount * sizeof(sgl_UniformBuffer*));
    pp->uniformCount = uniformCount;
    pp->gpu = device;
    pp->shader = sgl_Shader_Create(device, device->screenQuadLayout);
    return pp;
}

void sgl_PostProcess_AddUniformBuffer(sgl_PostProcess* pp, sgl_UniformBuffer* buffer, size_t index)
{
    if (index > pp->uniformCount)
        return;

    pp->uniforms[index] = buffer;
}

sgl_Shader* sgl_PostProcess_GetShader(sgl_PostProcess* pp) {
    return pp->shader;
}

void sgl_PostProcess_Bind(sgl_PostProcess* pp)
{
    pp->vtable->Bind(pp);

    for (size_t i = 0; i < pp->uniformCount; ++i)
        sgl_UniformBuffer_Bind(pp->uniforms[i], (uint32)i);
}

void sgl_PostProcess_OnResize(sgl_PostProcess* pp) {
    pp->vtable->OnResize(pp);
}

void sgl_PostProcess_Destroy(sgl_PostProcess* pp)
{
    sgl_Free(pp->uniforms);
    pp->vtable->Destroy(pp);
}

void sgl_PostProcess_SetEnabled(sgl_PostProcess* pp, bool enabled) {
    pp->enabled = enabled;
}

bool sgl_PostProcess_GetEnabled(sgl_PostProcess* pp) {
    return pp->enabled;
}