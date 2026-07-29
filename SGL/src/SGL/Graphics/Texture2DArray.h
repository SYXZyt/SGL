#pragma once
#include <SGL/Graphics/Texture.h>

SGL_BEGIN

/// @brief 2D texture array. Features multiple subimages. This requires a 3D uv coord with z being image index
typedef struct sgl_Texture2DArray sgl_sealed
{
    sgl_Texture base;
    sgl_Vec2i frameSize;
    uint32 textureCount;
} sgl_Texture2DArray;

/// @brief Create a new 2D texture array from a file
/// @param device The device to use
/// @param path The path of the texture to load
/// @param frameSize How big each sub image is. Each sub image must be the same size.
/// @return The loaded texture
SGL_API extern sgl_Texture* sgl_Texture2DArray_New_File(sgl_GraphicsDevice* device, const char* path, sgl_Vec2i frameSize);

/// @param frameSize 
/// @brief Create a new 2D texture from binary file contents (not binary image data)
/// @param device The device to use
/// @param data Pointer to file contents
/// @param dataSize How many bytes to read
/// @param frameSize How big each sub image is. Each sub image must be the same size.
/// @return The loaded texture
SGL_API extern sgl_Texture* sgl_Texture2DArray_New_Source(sgl_GraphicsDevice* device, void* data, size_t dataSize, sgl_Vec2i frameSize);

SGL_END