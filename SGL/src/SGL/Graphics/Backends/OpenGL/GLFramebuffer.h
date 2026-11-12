#pragma once
#include <SGL/Graphics/Framebuffer.h>
#include <glad/glad.h>

SGL_BEGIN

/// @brief OpenGL offscreen render target
typedef struct sgl_GLFramebuffer sgl_sealed
{
    sgl_Framebuffer base;

    gluint fbo;
    gluint texture;

    // Only valid when base.hasDepth
    gluint depthTexture;
} sgl_GLFramebuffer;

/// @brief Create an OpenGL offscreen render target
SGL_API extern sgl_GLFramebuffer* sgl_GLFramebuffer_Create(sgl_GraphicsDevice* device, sgl_Vec2i size, bool hasDepth, sgl_FramebufferFormat format);

SGL_END