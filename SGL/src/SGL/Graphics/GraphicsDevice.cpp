#include "GraphicsDevice.h"
#include <SGL/Util/Error.h>
#include <SGL/Graphics/Backends/OpenGL/GLDevice.h>

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

void sgl_GraphicsDevice_Draw(sgl_GraphicsDevice* device, struct sgl_VertexArray* va, struct sgl_Shader* shader) {
    device->vtable->Draw(device, va, shader);
}