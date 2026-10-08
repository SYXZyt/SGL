#pragma once
#include <SGL/SGL.h>
#include <SGL/Maths/Vec2.h>
#include <SGL/Graphics/GraphicsDevice.h>
#include <SGL/Graphics/Sampler.h>

SGL_BEGIN

struct sgl_Texture;
typedef struct sgl_TextureVTable sgl_sealed
{
    void (*Destroy)(struct sgl_Texture* texture);
    void (*Bind)(struct sgl_Texture* texture, uint32 unit);
} sgl_TextureVTable;

/// @brief Abstract template interface. You cannot instantiate this directly
typedef struct sgl_Texture sgl_sealed
{
    const sgl_TextureVTable* vtable;
    sgl_GraphicsDevice* gpu;
    sgl_Sampler* sampler;
    sgl_Vec2i size;

    /// @brief Checks if the data has been created on the GPU. Bind (which must run on main thread) will upload data
    bool gpuLoaded;
} sgl_Texture;

/// @brief Destroy a texture
/// @param texture 
SGL_API extern void sgl_Texture_Destroy(sgl_Texture* texture);

/// @brief Bind a texture
/// @param texture The texture to bind
/// @param unit Which texture to bind into
SGL_API extern void sgl_Texture_Bind(sgl_Texture* texture, uint32 unit);

SGL_END