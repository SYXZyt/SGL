#include "GLDevice.h"
#include <SGL/Util/Memory.h>
#include <glad/glad.h>
#include <sstream>
#include <SGL/Util/Logger.h>
#include <SGL/Graphics/VertexArray.h>
#include <SGL/Graphics/Shader.h>

template <typename T>
T* GetBackend(void* ptr) {
    return reinterpret_cast<T*>(ptr);
}

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

static void GLDevice_Draw(sgl_GraphicsDevice* dev, sgl_VertexArray* va, sgl_Shader* shr)
{
    GetSelf;

    sgl_Shader_Bind(shr);
    sgl_VertexArray_Bind(va);

    glDrawArrays(GL_TRIANGLES, 0, va->vertexCount);
}

static const sgl_GraphicsDeviceVTable gGlVTable =
{
    .SetClearColour = &GLDevice_SetClearColour,
    .Resize = &GLDevice_Resize,
    .Clear = &GLDevice_Clear,
    .Present = &GLDevice_Present,
    .Destroy = &GLDevice_Destroy,
    .Draw = &GLDevice_Draw,
};

sgl_GLDevice* sgl_GLDevice_Create(sgl_Window* window)
{
    sgl_GLDevice* device = sgl::Memory::New<sgl_GLDevice>();
    device->base.clearColour = sgl_Col_CornflowerBlue;
    device->base.window = window;
    device->base.width = window->screenSize.width;
    device->base.height = window->screenSize.height;
    device->base.vtable = &gGlVTable;

    std::stringstream ss;

    ss << "SDL Version: " << SDL_VERSIONNUM_MAJOR(SDL_VERSION) << "." << SDL_VERSIONNUM_MINOR(SDL_VERSION) << "." << SDL_VERSIONNUM_MICRO(SDL_VERSION);
    sgl_Log(ss.str().c_str());
    ss.str("");

    ss << "OpenGL Version: " << (const char*)glGetString(GL_VERSION);
    sgl_Log(ss.str().c_str());
    ss.str("");

    ss << "GLSL Version: " << (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
    sgl_Log(ss.str().c_str());
    ss.str("");

    ss << "Vendor: " << (const char*)glGetString(GL_VENDOR);
    sgl_Log(ss.str().c_str());
    ss.str("");

    ss << "Renderer: " << (const char*)glGetString(GL_RENDERER);
    sgl_Log(ss.str().c_str());

    return device;
}