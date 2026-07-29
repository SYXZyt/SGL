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

struct sgl_UniformBuffer;

/// @brief A post processing effect
typedef struct sgl_PostProcess sgl_sealed
{
    const sgl_PostProcessVTable* vtable;
    sgl_GraphicsDevice* gpu;
    sgl_Shader* shader;
    bool enabled;
    struct sgl_UniformBuffer** uniforms;
    size_t uniformCount;
} sgl_PostProcess;

/// @brief Create a new post processing effect
/// @param device The device to use
/// @param uniformCount How many uniforms this effects expects
SGL_API extern sgl_PostProcess* sgl_PostProcess_Create(sgl_GraphicsDevice* device, size_t uniformCount);

/// @brief Register a uniform buffer for a post processing effect
/// @param pp The effect to register with
/// @param buffer The uniform buffer to register
/// @param index Which uniform buffer to use (which register in the shader)
SGL_API extern void sgl_PostProcess_AddUniformBuffer(sgl_PostProcess* pp, struct sgl_UniformBuffer* buffer, size_t index);

/// @brief Get the shader that an effect uses
/// @param pp The effect to get the shader of
SGL_API extern sgl_Shader* sgl_PostProcess_GetShader(sgl_PostProcess* pp);

/// @brief Bind a post processing effect
/// @param pp The effect to bind
SGL_API extern void sgl_PostProcess_Bind(sgl_PostProcess* pp);

/// @brief Resize the internal texture of a post processing effect (DO NOT CALL. The graphics device will automatically call this)
/// @param pp 
SGL_API extern void sgl_PostProcess_OnResize(sgl_PostProcess* pp);

/// @brief Destroy a post processing effect
/// @param pp The effect to destroy
SGL_API extern void sgl_PostProcess_Destroy(sgl_PostProcess* pp);

/// @brief Set whether a post processing effect should be used or not
/// @param pp The effect to dis/enable
/// @param enabled true to enable the effect, false to disable
SGL_API extern void sgl_PostProcess_SetEnabled(sgl_PostProcess* pp, bool enabled);

/// @brief Check if a post processing effect is active or not
/// @param pp The effect to check
/// @return True if the effect is active, or false otherwise
SGL_API extern bool sgl_PostProcess_GetEnabled(sgl_PostProcess* pp);

SGL_END