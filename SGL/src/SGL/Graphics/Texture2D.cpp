#include "Texture2D.h"
#include <stb/stb_image.h>
#include <SGL/Util/Error.h>
#include <SGL/Graphics/Backends/OpenGL/GLTexture2D.h>
#include <fstream>
#include <filesystem>
#include <vector>
#ifdef SGL_DIRECTX
#include <SGL/Graphics/Backends/DirectX/DXTexture2D.h>
#endif

sgl_Texture* sgl_Texture2D_New_File(sgl_GraphicsDevice* device, const char* path, bool generateMipmaps, sgl_TextureFilter filter, sgl_TextureClamp clamp)
{
    if (!std::filesystem::exists(path))
    {
        SGL_REPORT_ERROR((std::string("Could not find file: ") + path).c_str());
        return nullptr;
    }

    std::ifstream f(path, std::ios::binary);
    std::vector<uint8> data((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());

    return sgl_Texture2D_New_Source(device, data.data(), data.size(), generateMipmaps, filter, clamp);
}

sgl_Texture* sgl_Texture2D_New_Source(sgl_GraphicsDevice* device, void* data, size_t dataSize, bool generateMipmaps, sgl_TextureFilter filter, sgl_TextureClamp clamp)
{
    sgl_Texture* texture = nullptr;

    int width, height;
    stbi_set_flip_vertically_on_load_thread(true);

    void* bytes = stbi_load_from_memory((const stbi_uc*)data, (int)dataSize, &width, &height, nullptr, 4);
    if (!bytes)
    {
        SGL_REPORT_ERROR("Failed to decode texture data");
        return nullptr;
    }

    // Ownership of 'bytes' passes to the backend texture, which uploads it to the GPU on first Bind
    if (device->window->cfg.backend == sgl_Backend_OPENGL)
    {
        texture = (sgl_Texture*)sgl_GLTexture2D_Create(bytes, { {{width, height}} }, generateMipmaps);
    }
    else if (device->window->cfg.backend == sgl_Backend_DIRECTX11)
    {
#ifdef SGL_DIRECTX
        texture = (sgl_Texture*)sgl_DXTexture2D_Create(bytes, { {{width, height}} }, generateMipmaps);
#else
        SGL_REPORT_ERROR("DirectX is not supported on this platform");
        stbi_image_free(bytes);
        return nullptr;
#endif
    }
    else
    {
        SGL_REPORT_ERROR("Unsupported backend");
        stbi_image_free(bytes);
        return nullptr;
    }

    texture->sampler = sgl_Sampler_Create(device, filter, clamp);

    texture->gpu = device;
    return texture;
}

sgl_Texture* sgl_Texture2D_New_Raw(sgl_GraphicsDevice* device, void* pixels, sgl_Vec2i size, bool generateMipmaps, sgl_TextureFilter filter, sgl_TextureClamp clamp)
{
    sgl_Texture* texture = nullptr;

    if (device->window->cfg.backend == sgl_Backend_OPENGL)
    {
        texture = (sgl_Texture*)sgl_GLTexture2D_Create(pixels, size, generateMipmaps);
    }
    else if (device->window->cfg.backend == sgl_Backend_DIRECTX11)
    {
#ifdef SGL_DIRECTX
        texture = (sgl_Texture*)sgl_DXTexture2D_Create(pixels, size, generateMipmaps);
#else
        SGL_REPORT_ERROR("DirectX is not supported on this platform");
        stbi_image_free(pixels);
        return nullptr;
#endif
    }
    else
    {
        SGL_REPORT_ERROR("Unsupported backend");
        stbi_image_free(pixels);
        return nullptr;
    }

    texture->sampler = sgl_Sampler_Create(device, filter, clamp);

    texture->gpu = device;
    return texture;
}