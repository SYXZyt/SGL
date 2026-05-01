#include "GLDevice.h"
#include <glad/glad.h>
#include <sgl/Game.h>

void sgl::Graphics::GLDevice::Resize(const Vec2i& newsize)
{
    mWidth = newsize.width;
    mHeight = newsize.height;

    glViewport(0, 0, mWidth, mHeight);
}

void sgl::Graphics::GLDevice::SetClearColour(const Colour &colour)
{
    GraphicsDevice::SetClearColour(colour);
    glClearColor(colour.r, colour.g, colour.b, colour.a);
}

void sgl::Graphics::GLDevice::Clear(ClearFlag flag)
{
    GLbitfield glFlags = 0;
    if ((uint8)(flag & ClearFlag::COLOUR) != 0)
        glFlags |= GL_COLOR_BUFFER_BIT;

    if ((uint8)(flag & ClearFlag::DEPTH) != 0)
        glFlags |= GL_DEPTH_BUFFER_BIT;

    glClear(glFlags);
}

void sgl::Graphics::GLDevice::Present()
{
    SDL_GL_SwapWindow(mWindow);
}

void sgl::Graphics::GLDevice::Init()
{
    glEnable(GL_DEPTH_TEST);

    mWindow = Get<Game>().GetWindow().SDLWindow();

    const Vec2i& size = Get<Game>().GetWindow().ScreenSize();
    mWidth = size.width;
    mHeight = size.height;

    glViewport(0, 0, mWidth, mHeight);
}

void sgl::Graphics::GLDevice::Shutdown()
{
}