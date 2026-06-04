#pragma once
#include <SGL/SGL.h>
#include <SGL/Graphics/Shader.h>
#include <SGL/Graphics/GraphicsDevice.h>

SGL_BEGIN

struct sgl_PostProcess;

typedef struct sgl_PostProcessVTable sgl_sealed
{
    void (*Destroy)(struct sgl_PostProcess* self);
    void (*Bind)(struct sgl_PostProcess* self);
    void (*OnResize)(struct sgl_PostProcess* self);
} sgl_PostProcessVTable;

typedef struct sgl_PostProcess sgl_sealed
{
    const sgl_PostProcessVTable* vtable;
    sgl_GraphicsDevice* gpu;
    sgl_Shader* shader;
    bool enabled;
} sgl_PostProcess;

SGL_API extern sgl_PostProcess* sgl_PostProcess_Create(sgl_GraphicsDevice* device, sgl_Shader* shader);

SGL_API extern void sgl_PostProcess_Bind(sgl_PostProcess* pp);
SGL_API extern void sgl_PostProcess_OnResize(sgl_PostProcess* pp);  // <-- The graphics pipeline will call this automatically, you shouldn't ever need to do it manually
SGL_API extern void sgl_PostProcess_Destroy(sgl_PostProcess* pp);

SGL_END