#pragma once
#include <SGL/SGL.h>
#include <SGL/Maths/Vec2.h>
#include <SGL/Graphics/GraphicsDevice.h>
#include <SGL/Graphics/Texture.h>

SGL_BEGIN

typedef struct sgl_Texture2D sgl_sealed
{
    sgl_Texture base;

    /// @brief CPU-side decoded pixel data. Waiting for GPU upload
    void* pixels;
} sgl_Texture2D;

/// @brief Create a new 2D texture from a file
/// @param device The device to use
/// @param path The path of the texture to use
/// @param premultiplyAlpha Multiply RGB by alpha on load. Correct for colour+opacity textures. Fixes an issue with mips
/// @return The loaded texture
SGL_API extern sgl_Texture* sgl_Texture2D_New_File(sgl_GraphicsDevice* device, const char* path, bool premultiplyAlpha);

/// @brief Create a new 2D texture from binary file contents (not binary image data)
/// @param device The device to use
/// @param data Pointer to file contents
/// @param dataSize How many bytes to read
/// @param premultiplyAlpha Multiply RGB by alpha on load. Correct for colour+opacity textures. Fixes an issue with mips
/// @return The loaded texture
SGL_API extern sgl_Texture* sgl_Texture2D_New_Source(sgl_GraphicsDevice* device, void* data, size_t dataSize, bool premultiplyAlpha);

SGL_END