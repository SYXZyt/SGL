#pragma once
#include <SGL/SGL.h>
#include <SGL/Graphics/GraphicsDevice.h>

SGL_BEGIN

/// @brief Specifies what filter algorithm to use for a texture
typedef uint8 sgl_TextureFilter;
enum
{
    /// @brief Linear filtering. Smoothly interpolate between pixels. Results in a blury image. Best used for high-res images
    sgl_TextureFilter_LINEAR,

    /// @brief Nearest neighbour. No interpolation, only solid pixel colours. Results in sharp, pixelated image. Best used for pixel art
    sgl_TextureFilter_NEAREST,
};

/// @brief Specifies what address mode to use for texture borders
typedef uint8 sgl_TextureClamp;
enum
{
    /// @brief Repeat the texture
    sgl_TextureClamp_REPEAT,

    /// @brief Repeat the texture but mirrored
    sgl_TextureClamp_MIRROR,

    /// @brief Lock the texture and repeat what ever pixel colour fell on the border
    sgl_TextureClamp_EDGE,
};

struct sgl_Sampler;
typedef struct sgl_SamplerVTable sgl_sealed
{
    void (*Destroy)(struct sgl_Sampler* sampler);
    void (*Bind)(struct sgl_Sampler* sampler, uint32 unit);
} sgl_SamplerVTable;

typedef struct sgl_Sampler sgl_sealed
{
    const sgl_SamplerVTable* vtable;
    sgl_GraphicsDevice* gpu;
    sgl_TextureFilter filter;
    sgl_TextureClamp clamp;

    /// @brief Checks if the data has been created on the GPU. Bind (which must run on main thread) will upload data
    bool gpuLoaded;
} sgl_Sampler;

SGL_API extern sgl_Sampler* sgl_Sampler_Create(sgl_GraphicsDevice* gpu, sgl_TextureFilter filter, sgl_TextureClamp clamp);
SGL_API extern void sgl_Sampler_Destroy(sgl_Sampler* sampler);

SGL_API extern void sgl_Sampler_Bind(sgl_Sampler* sampler, uint32 unit);

SGL_END