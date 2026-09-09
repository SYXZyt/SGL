#include "GLSampler.h"
#include <glad/glad.h>
#include <SGL/Util/Memory.h>
#include <SGL/Graphics/Backends/OpenGL/OpenGLThreadSync.h>

static void EnsureGLGPUResources(sgl_GLSampler* self)
{
    self->base.gpuLoaded = true;

    glGenSamplers(1, &self->samplerId);

    switch (self->base.filter)
    {
        case sgl_TextureFilter_LINEAR:
            glSamplerParameteri(self->samplerId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glSamplerParameteri(self->samplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;
        case sgl_TextureFilter_NEAREST:
        default:
            glSamplerParameteri(self->samplerId, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            glSamplerParameteri(self->samplerId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;
    }

    switch (self->base.clamp)
    {
        case sgl_TextureClamp_REPEAT:
            glSamplerParameteri(self->samplerId, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glSamplerParameteri(self->samplerId, GL_TEXTURE_WRAP_T, GL_REPEAT);
            break;
        case sgl_TextureClamp_MIRROR:
            glSamplerParameteri(self->samplerId, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            glSamplerParameteri(self->samplerId, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            break;
        case sgl_TextureClamp_EDGE:
        default:
            glSamplerParameteri(self->samplerId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glSamplerParameteri(self->samplerId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            break;
    }
}

#define GetSelf sgl_GLSampler* self = (sgl_GLSampler*)sampler

static void GLBind(sgl_Sampler* sampler, uint32 unit)
{
    GetSelf;

    if (!sampler->gpuLoaded)
        EnsureGLGPUResources(self);

    glBindSampler(unit, self->samplerId);
}

static void GLDestroy(sgl_Sampler* sampler)
{
    GetSelf;
    sgl_OpenGLThreadSync_DeleteSampler(self->samplerId);
    sgl::Memory::Delete(self);
}

static const sgl_SamplerVTable gGLVTable =
{
    .Destroy = &GLDestroy,
    .Bind = &GLBind,
};

sgl_GLSampler* sgl_GLSampler_Create(sgl_TextureFilter filter, sgl_TextureClamp clamp)
{
    sgl_GLSampler* sampler = sgl::Memory::New<sgl_GLSampler>();

    sampler->base.clamp = clamp;
    sampler->base.filter = filter;
    sampler->base.vtable = &gGLVTable;
    sampler->base.gpuLoaded = false;

    return sampler;
}