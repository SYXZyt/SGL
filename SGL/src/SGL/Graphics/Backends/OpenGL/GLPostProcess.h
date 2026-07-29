#pragma once
#include <SGL/Graphics/PostProcess.h>
#include <glad/glad.h>

SGL_BEGIN

/// @brief OpenGL post processing effect
typedef struct sgl_GLPostProcess sgl_sealed
{
    sgl_PostProcess base;
    gluint texture;
    gluint framebuffer;

} sgl_GLPostProcess;

/// @brief Create an OpenGL post processing effect
SGL_API extern sgl_GLPostProcess* sgl_GLPostProcess_Create(sgl_GraphicsDevice* device);

SGL_END