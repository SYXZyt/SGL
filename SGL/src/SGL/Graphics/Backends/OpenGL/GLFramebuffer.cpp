#include "GLFramebuffer.h"
#include <SGL/Util/Memory.h>
#include <SGL/Util/Error.h>
#include <SGL/Graphics/Texture.h>
#include <SGL/Graphics/Sampler.h>
#include <SGL/Graphics/Backends/OpenGL/OpenGLThreadSync.h>

#define GetSelf sgl_GLFramebuffer* self = (sgl_GLFramebuffer*)fb

typedef struct sgl_GLFramebufferTexture sgl_sealed
{
    sgl_Texture base;
    sgl_GLFramebuffer* owner;
} sgl_GLFramebufferTexture;

static void GLFramebufferTexture_Destroy(sgl_Texture*) {
}

static void GLFramebufferTexture_Bind(sgl_Texture* tex, uint32 unit)
{
    sgl_GLFramebufferTexture* self = (sgl_GLFramebufferTexture*)tex;
    glBindTextureUnit(unit, self->owner->texture);
    sgl_Sampler_Bind(tex->sampler, unit);
}

static sgl_TextureVTable gGLFramebufferTextureVTable =
{
    .Destroy = &GLFramebufferTexture_Destroy,
    .Bind = &GLFramebufferTexture_Bind,
};

static void GL_ReleaseTargets(sgl_GLFramebuffer* self)
{
    sgl_OpenGLThreadSync_DeleteTexture(self->texture);
    self->texture = 0;

    if (self->depthTexture)
    {
        sgl_OpenGLThreadSync_DeleteTexture(self->depthTexture);
        self->depthTexture = 0;
    }
}

static GLenum GL_ToFormat(sgl_FramebufferFormat format)
{
    switch (format)
    {
        case sgl_FramebufferFormat_RGBA16F: return GL_RGBA16F;
        case sgl_FramebufferFormat_R8: return GL_R8;
        case sgl_FramebufferFormat_RGBA8:
        default: return GL_RGBA8;
    }
}

static void GL_CreateTargets(sgl_GLFramebuffer* self, sgl_Vec2i size)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &self->texture);
    glTextureStorage2D(self->texture, 1, GL_ToFormat(self->base.format), size.width, size.height);
    glTextureParameteri(self->texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(self->texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(self->texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(self->texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glNamedFramebufferTexture(self->fbo, GL_COLOR_ATTACHMENT0, self->texture, 0);

    if (self->base.hasDepth)
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &self->depthTexture);
        glTextureStorage2D(self->depthTexture, 1, GL_DEPTH_COMPONENT24, size.width, size.height);
        glNamedFramebufferTexture(self->fbo, GL_DEPTH_ATTACHMENT, self->depthTexture, 0);
    }

    const GLenum status = glCheckNamedFramebufferStatus(self->fbo, GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
        SGL_REPORT_ERROR("Failed to create framebuffer");
}

static void GL_Destroy(sgl_Framebuffer* fb)
{
    GetSelf;
    GL_ReleaseTargets(self);
    sgl_OpenGLThreadSync_DeleteFramebuffer(self->fbo);
    sgl::Memory::Delete((sgl_GLFramebufferTexture*)self->base.colourTexture);
    sgl::Memory::Delete(self);
}

static void GL_Resize(sgl_Framebuffer* fb, sgl_Vec2i size)
{
    GetSelf;
    GL_ReleaseTargets(self);
    self->base.size = size;
    self->base.colourTexture->size = size;
    GL_CreateTargets(self, size);
}

static void GL_Bind(sgl_Framebuffer* fb)
{
    GetSelf;
    glBindFramebuffer(GL_FRAMEBUFFER, self->fbo);
    glViewport(0, 0, (GLsizei)fb->size.width, (GLsizei)fb->size.height);
}

static void GL_Clear(sgl_Framebuffer* fb, sgl_Colour colour)
{
    GetSelf;
    glBindFramebuffer(GL_FRAMEBUFFER, self->fbo);

    const float clearColour[4] = { colour.r, colour.g, colour.b, colour.a };
    glClearNamedFramebufferfv(self->fbo, GL_COLOR, 0, clearColour);

    if (self->base.hasDepth)
    {
        const float depth = 1.0f;
        glClearNamedFramebufferfv(self->fbo, GL_DEPTH, 0, &depth);
    }
}

static sgl_FramebufferVTable gGLVTable =
{
    .Destroy = &GL_Destroy,
    .Resize = &GL_Resize,
    .Bind = &GL_Bind,
    .Clear = &GL_Clear,
};

sgl_GLFramebuffer* sgl_GLFramebuffer_Create(sgl_GraphicsDevice* device, sgl_Vec2i size, bool hasDepth, sgl_FramebufferFormat format)
{
    sgl_GLFramebuffer* framebuffer = sgl::Memory::New<sgl_GLFramebuffer>();
    framebuffer->base.vtable = &gGLVTable;
    framebuffer->base.gpu = device;
    framebuffer->base.size = size;
    framebuffer->base.hasDepth = hasDepth;
    framebuffer->base.format = format;

    glCreateFramebuffers(1, &framebuffer->fbo);
    GL_CreateTargets(framebuffer, size);

    sgl_GLFramebufferTexture* texture = sgl::Memory::New<sgl_GLFramebufferTexture>();
    texture->base.vtable = &gGLFramebufferTextureVTable;
    texture->base.gpu = device;
    texture->base.sampler = nullptr;
    texture->base.size = size;
    texture->base.gpuLoaded = true;
    texture->owner = framebuffer;

    framebuffer->base.colourTexture = (sgl_Texture*)texture;

    return framebuffer;
}