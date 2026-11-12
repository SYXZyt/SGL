#include "Framebuffer.h"
#include <SGL/Graphics/Backends/OpenGL/GLFramebuffer.h>
#include <SGL/Graphics/Backends/DirectX/DXFramebuffer.h>
#include <SGL/Graphics/Texture.h>
#include <SGL/Graphics/Sampler.h>
#include <SGL/Util/Error.h>

sgl_Framebuffer* sgl_Framebuffer_Create(sgl_GraphicsDevice* device, sgl_Vec2i size, bool hasDepth) {
    return sgl_Framebuffer_CreateEx(device, size, hasDepth, sgl_FramebufferFormat_RGBA8, false);
}

sgl_Framebuffer* sgl_Framebuffer_CreateEx(sgl_GraphicsDevice* device, sgl_Vec2i size, bool hasDepth, sgl_FramebufferFormat format, bool linearFilter)
{
    sgl_Framebuffer* framebuffer = nullptr;

    if (device->window->cfg.backend == sgl_Backend_OPENGL)
    {
        framebuffer = (sgl_Framebuffer*)sgl_GLFramebuffer_Create(device, size, hasDepth, format);
    }
    else if (device->window->cfg.backend == sgl_Backend_DIRECTX11)
    {
#ifdef SGL_DIRECTX
        framebuffer = (sgl_Framebuffer*)sgl_DXFramebuffer_Create(device, size, hasDepth, format);
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

    framebuffer->gpu = device;
    framebuffer->colourTexture->sampler = sgl_Sampler_Create(device, linearFilter ? sgl_TextureFilter_LINEAR : sgl_TextureFilter_NEAREST, sgl_TextureClamp_EDGE);

    return framebuffer;
}

void sgl_Framebuffer_Destroy(sgl_Framebuffer* framebuffer) {
    sgl_Sampler_Destroy(framebuffer->colourTexture->sampler);
    framebuffer->vtable->Destroy(framebuffer);
}

void sgl_Framebuffer_Resize(sgl_Framebuffer* framebuffer, sgl_Vec2i size)
{
    size.width = size.width < 1 ? 1 : size.width;
    size.height = size.height < 1 ? 1 : size.height;

    if (framebuffer->size.width == size.width && framebuffer->size.height == size.height)
        return;

    framebuffer->vtable->Resize(framebuffer, size);
}

void sgl_Framebuffer_Bind(sgl_Framebuffer* framebuffer) {
    framebuffer->vtable->Bind(framebuffer);
}

void sgl_Framebuffer_Clear(sgl_Framebuffer* framebuffer, sgl_Colour colour) {
    framebuffer->vtable->Clear(framebuffer, colour);
}

sgl_Texture* sgl_Framebuffer_GetTexture(sgl_Framebuffer* framebuffer) {
    return framebuffer->colourTexture;
}

sgl_Vec2i sgl_Framebuffer_GetSize(sgl_Framebuffer* framebuffer) {
    return framebuffer->size;
}