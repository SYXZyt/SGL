#include "PostProcess.h"
#include <SGL/Graphics/Framebuffer.h>
#include <SGL/Graphics/Shader.h>
#include <SGL/Graphics/UniformBuffer.h>
#include <SGL/Util/Memory.h>

sgl_PostProcess* sgl_PostProcess_Create(sgl_GraphicsDevice* device, size_t uniformCount)
{
    sgl_PostProcess* pp = sgl::Memory::New<sgl_PostProcess>();
    pp->enabled = true;
    pp->gpu = device;
    pp->shader = sgl_Shader_Create(device, device->screenQuadLayout);
    pp->target = sgl_Framebuffer_Create(device, device->window->screenSize, false);
    pp->uniforms = (sgl_UniformBuffer**)sgl_Malloc(uniformCount * sizeof(sgl_UniformBuffer*));
    pp->uniformCount = uniformCount;
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

sgl_Framebuffer* sgl_PostProcess_GetTarget(sgl_PostProcess* pp) {
    return pp->target;
}

sgl_Texture* sgl_PostProcess_GetTexture(sgl_PostProcess* pp) {
    return sgl_Framebuffer_GetTexture(pp->target);
}

void sgl_PostProcess_Bind(sgl_PostProcess* pp)
{
    sgl_Framebuffer_Bind(pp->target);
    sgl_Shader_Bind(pp->shader);

    for (size_t i = 0; i < pp->uniformCount; ++i)
        sgl_UniformBuffer_Bind(pp->uniforms[i], (uint32)i);
}

void sgl_PostProcess_OnResize(sgl_PostProcess* pp) {
    sgl_Framebuffer_Resize(pp->target, pp->gpu->window->screenSize);
}

void sgl_PostProcess_Destroy(sgl_PostProcess* pp)
{
    sgl_Free(pp->uniforms);
    sgl_Framebuffer_Destroy(pp->target);
    sgl_Shader_Destroy(pp->shader);
    sgl::Memory::Delete(pp);
}

void sgl_PostProcess_SetEnabled(sgl_PostProcess* pp, bool enabled) {
    pp->enabled = enabled;
}

bool sgl_PostProcess_GetEnabled(sgl_PostProcess* pp) {
    return pp->enabled;
}