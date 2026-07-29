#include "Texture2DArray.h"
#include <fstream>
#include <filesystem>
#include <SGL/Util/Error.h>
#include <stb/stb_image.h>
#include <SGL/Graphics/Backends/OpenGL/GLTexture2DArray.h>
#include <SGL/Graphics/Backends/DirectX/DXTexture2DArray.h>

sgl_Texture* sgl_Texture2DArray_New_File(sgl_GraphicsDevice* device, const char* path, sgl_Vec2i frameSize)
{
    if (!std::filesystem::exists(path))
    {
        SGL_REPORT_ERROR((std::string("Could not find file: ") + path).c_str());
        return nullptr;
    }

    std::ifstream f(path, std::ios::binary);
    std::vector<uint8> data((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());

    return sgl_Texture2DArray_New_Source(device, data.data(), data.size(), frameSize);
}

sgl_Texture* sgl_Texture2DArray_New_Source(sgl_GraphicsDevice* device, void* data, size_t dataSize, sgl_Vec2i frameSize)
{
    sgl_Texture* texture = nullptr;

    int width, height;
    stbi_set_flip_vertically_on_load_thread(true);

    void* bytes = stbi_load_from_memory((const stbi_uc*)data, (int)dataSize, &width, &height, nullptr, 4);

    if (device->window->cfg.backend == sgl_Backend_OPENGL)
    {
        texture = (sgl_Texture*)sgl_GLTexture2DArray_Create(bytes, { {{width, height}} }, frameSize);
    }
    else if (device->window->cfg.backend == sgl_Backend_DIRECTX11)
    {
#ifdef SGL_DIRECTX
        texture = (sgl_Texture*)sgl_DXTexture2DArray_Create(device, bytes, { {{width, height}} }, frameSize);
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

    stbi_image_free(bytes);
    return texture;
}