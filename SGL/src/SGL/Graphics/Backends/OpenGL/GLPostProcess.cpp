#include "GLPostProcess.h"
#include <SGL/Util/Memory.h>
#include <SGL/Util/Error.h>

#define GetSelf sgl_GLPostProcess* self = (sgl_GLPostProcess*)pp

static void GL_Release(sgl_GLPostProcess* self)
{
    glDeleteTextures(1, &self->texture);
    glDeleteFramebuffers(1, &self->framebuffer);
}

static void GL_Init(sgl_GLPostProcess* self, sgl_Vec2i screenSize)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &self->texture);
    glTextureStorage2D(self->texture, 1, GL_RGBA8, screenSize.width, screenSize.height);

    glTextureParameteri(self->texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(self->texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(self->texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(self->texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glCreateFramebuffers(1, &self->framebuffer);
    glNamedFramebufferTexture(self->framebuffer, GL_COLOR_ATTACHMENT0, self->texture, 0);

    const GLenum status = glCheckNamedFramebufferStatus(self->framebuffer, GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
        SGL_REPORT_ERROR("Failed to create post-process framebuffer");
}

static void GL_Bind(sgl_PostProcess* pp)
{
    GetSelf;
    glBindFramebuffer(GL_FRAMEBUFFER, self->framebuffer);
    sgl_Shader_Bind(pp->shader);
}

static void GL_Destroy(sgl_PostProcess* pp)
{
    GetSelf;
    GL_Release(self);
    sgl_Shader_Destroy(pp->shader);
    sgl::Memory::Delete(self);
}

static void GL_OnResize(sgl_PostProcess* pp)
{
    GetSelf;
    GL_Release(self);
    GL_Init(self, pp->gpu->window->screenSize);
}

static sgl_PostProcessVTable gGLVTable =
{
    .Destroy = &GL_Destroy,
    .Bind = &GL_Bind,
    .OnResize = &GL_OnResize,
};

sgl_GLPostProcess* sgl_GLPostProcess_Create(sgl_GraphicsDevice* device, sgl_Shader* shader)
{
    sgl_GLPostProcess* pp = sgl::Memory::New<sgl_GLPostProcess>();
    pp->base.enabled = true;
    pp->base.vtable = &gGLVTable;

    GL_Init(pp, device->window->screenSize);

    return pp;
}