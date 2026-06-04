#include "Texture2D.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <SGL/Util/Error.h>
#include <SGL/Graphics/Backends/DirectX/DXTexture2D.h>
#include <SGL/Graphics/Backends/OpenGL/GLTexture2D.h>
#include <fstream>
#include <filesystem>

sgl_Texture2D* sgl_Texture2D_New_File(sgl_GraphicsDevice* device, const char* path)
{
    if (!std::filesystem::exists(path))
    {
        SGL_REPORT_ERROR((std::string("Could not find file: ") + path).c_str());
        return nullptr;
    }

    std::ifstream f(path, std::ios::binary);
    std::vector<uint8> data((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());

    return sgl_Texture2D_New_Source(device, data.data(), data.size());
}

sgl_Texture2D* sgl_Texture2D_New_Source(sgl_GraphicsDevice* device, void* data, size_t dataSize)
{
    sgl_Texture2D* texture = nullptr;

    int width, height;
    stbi_set_flip_vertically_on_load_thread(false);

    void* bytes = stbi_load_from_memory((const stbi_uc*)data, (int)dataSize, &width, &height, nullptr, 4);

    if (device->window->cfg.backend == sgl_Backend_OPENGL)
    {
        texture = (sgl_Texture2D*)sgl_GLTexture2D_Create(bytes, { {{width, height}} });
    }
    else if (device->window->cfg.backend == sgl_Backend_DIRECTX11)
    {
#ifdef SGL_DIRECTX
        texture = (sgl_Texture2D*)sgl_DXTexture2D_Create(device, bytes, { {{width, height}} });
#else
        SGL_REPORT_ERROR("DirectX is not supported on this platform");
        return nullptr;
#endif
    }
    else
    {
        SGL_REPORT_ERROR("Unsupported backend");
        return nullptr;
    }

    texture->gpu = device;
    return texture;
}

void sgl_Texture2D_Destroy(sgl_Texture2D* tex) {
    tex->vtable->Destroy(tex);
}

void sgl_Texture2D_Bind(sgl_Texture2D* tex, uint32 slot) {
    tex->vtable->Bind(tex, slot);
}