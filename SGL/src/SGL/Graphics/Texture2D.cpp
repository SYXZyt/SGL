#include "Texture2D.h"
#include <stb/stb_image.h>
#include <SGL/Util/Error.h>
#include <SGL/Graphics/Backends/DirectX/DXTexture2D.h>
#include <SGL/Graphics/Backends/OpenGL/GLTexture2D.h>
#include <fstream>
#include <filesystem>

sgl_Texture* sgl_Texture2D_New_File(sgl_GraphicsDevice* device, const char* path, bool premultiplyAlpha)
{
    if (!std::filesystem::exists(path))
    {
        SGL_REPORT_ERROR((std::string("Could not find file: ") + path).c_str());
        return nullptr;
    }

    std::ifstream f(path, std::ios::binary);
    std::vector<uint8> data((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());

    return sgl_Texture2D_New_Source(device, data.data(), data.size(), premultiplyAlpha);
}

sgl_Texture* sgl_Texture2D_New_Source(sgl_GraphicsDevice* device, void* data, size_t dataSize, bool premultiplyAlpha)
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

    if (premultiplyAlpha)
    {
        stbi_uc* pixels = (stbi_uc*)bytes;
        size_t pixelCount = (size_t)width * (size_t)height;
        for (size_t i = 0; i < pixelCount; ++i)
        {
            stbi_uc a = pixels[i * 4 + 3];
            pixels[i * 4 + 0] = (stbi_uc)((pixels[i * 4 + 0] * a) / 255);
            pixels[i * 4 + 1] = (stbi_uc)((pixels[i * 4 + 1] * a) / 255);
            pixels[i * 4 + 2] = (stbi_uc)((pixels[i * 4 + 2] * a) / 255);
        }
    }

    // Ownership of 'bytes' passes to the backend texture, which uploads it to the GPU on first Bind
    if (device->window->cfg.backend == sgl_Backend_OPENGL)
    {
        texture = (sgl_Texture*)sgl_GLTexture2D_Create(bytes, { {{width, height}} });
    }
    else if (device->window->cfg.backend == sgl_Backend_DIRECTX11)
    {
#ifdef SGL_DIRECTX
        texture = (sgl_Texture*)sgl_DXTexture2D_Create(bytes, { {{width, height}} });
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

    texture->gpu = device;
    return texture;
}