#pragma once
#include <SGL/Graphics/GraphicsDevice.h>

SGL_BEGIN

typedef struct sgl_GLDevice sgl_sealed
{
    sgl_GraphicsDevice base;
    gluint sceneFBO;
    gluint sceneTexture;
} sgl_GLDevice;

SGL_API extern sgl_GLDevice* sgl_GLDevice_Create(sgl_Window* window);

SGL_END