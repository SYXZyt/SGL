#include "GLDevice.h"
#include <SGL/Util/Memory.h>
#include <glad/glad.h>

#define GetSelf sgl_GLDevice* self = (sgl_GLDevice*)dev

static void GLDevice_SetClearColour(sgl_GraphicsDevice* dev, sgl_Colour colour)
{
    dev->clearColour = colour;
    glClearColor(colour.r, colour.g, colour.b, colour.a);
}

static void GLDevice_Resize(sgl_GraphicsDevice* dev, sgl_Vec2i newSize)
{
    dev->width = newSize.width;
    dev->height = newSize.height;
}

static void GLDevice_Clear(sgl_GraphicsDevice* dev, sgl_ClearFlags clearFlags)
{
    GetSelf;
}

static void GLDevice_Present(sgl_GraphicsDevice* dev) {
    sgl_Window_SwapBuffer(dev->window);
}

static void GLDevice_Destroy(sgl_GraphicsDevice* dev)
{
    GetSelf;

    sgl::Memory::Delete(self);
}

static const sgl_GraphicsDeviceVTable gGlVTable =
{
    .setClearColour = &GLDevice_SetClearColour,
    .resize = &GLDevice_Resize,
    .clear = &GLDevice_Clear,
    .present = &GLDevice_Present,
    .destroy = &GLDevice_Destroy,
};

sgl_GLDevice* sgl_GLDevice_Create(sgl_Window* window)
{
    sgl_GLDevice* device = sgl::Memory::New<sgl_GLDevice>();
    device->base.clearColour = sgl_Col_CornflowerBlue;
    device->base.window = window;
    device->base.width = window->screenSize.width;
    device->base.height = window->screenSize.height;
    device->base.vtable = &gGlVTable;

    return device;
}