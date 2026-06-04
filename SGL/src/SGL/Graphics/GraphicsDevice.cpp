#include "GraphicsDevice.h"
#include <SGL/Util/Error.h>
#include <SGL/Graphics/Backends/OpenGL/GLDevice.h>
#include <SGL/Graphics/Backends/DirectX/DXDevice.h>

static void OnResize(sgl_Window* window, sgl_Vec2i newSize, void* userdata)
{
    sgl_GraphicsDevice* device = (sgl_GraphicsDevice*)userdata;
    device->vtable->Resize(device, newSize);
}

sgl_GraphicsDevice* sgl_GraphicsDevice_Create(sgl_Window* window)
{
    sgl_GraphicsDevice* device = nullptr;

    if (window->cfg.backend == sgl_Backend_OPENGL)
    {
        device = (sgl_GraphicsDevice*)sgl_GLDevice_Create(window);
    }
    else if (window->cfg.backend == sgl_Backend_DIRECTX11)
    {
#ifdef SGL_DIRECTX
        device = (sgl_GraphicsDevice*)sgl_DXDevice_Create(window);
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

    sgl_Window_RegisterResize(window, &OnResize, device);
    return device;
}

void sgl_GraphicsDevice_Destroy(sgl_GraphicsDevice* device)
{
    sgl_Window_DeregisterResize(device->window, &OnResize);
    device->vtable->Destroy(device);
}

void sgl_GraphicsDevice_SetClearColour(sgl_GraphicsDevice* device, sgl_Colour colour) {
    device->vtable->SetClearColour(device, colour);
}

void sgl_GraphicsDevice_Clear(sgl_GraphicsDevice* device) {
    device->vtable->Clear(device);
}

void sgl_GraphicsDevice_Present(sgl_GraphicsDevice* device) {
    device->vtable->Present(device);
}

void sgl_GraphicsDevice_Draw(sgl_GraphicsDevice* device, struct sgl_VertexArray* va, struct sgl_Shader* shader, struct sgl_Texture2D** textures, size_t textureCount, struct sgl_UniformBuffer** buffers, size_t bufferCount) {
    device->vtable->Draw(device, va, shader, textures, textureCount, buffers, bufferCount);
}

void sgl_GraphicsDevice_ImGui_Init(sgl_GraphicsDevice* device) {
    device->vtable->ImGui_Init(device);
}

void sgl_GraphicsDevice_ImGui_Shutdown(sgl_GraphicsDevice* device) {
    device->vtable->ImGui_Shutdown(device);
}

void sgl_GraphicsDevice_ImGui_NewFrame(sgl_GraphicsDevice* device) {
    device->vtable->ImGui_NewFrame(device);
}

void sgl_GraphicsDevice_ImGui_RenderDrawData(sgl_GraphicsDevice* device) {
    device->vtable->ImGui_RenderDrawData(device);
}