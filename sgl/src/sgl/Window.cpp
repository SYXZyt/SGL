#include "Window.h"
#include <sgl/Error.h>

const sgl::Window::Config& sgl::Window::Config::Default = {};

void sgl::Window::PollEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_EVENT_QUIT:
                mWantClose = true;
                break;
        }
    }
}

sgl::Window::Window(const Config& cfg)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SGL_REPORT_ERROR(u8"Failed to init sdl");
        return;
    }

    mScreenSize.width = (float)cfg.width;
    mScreenSize.height = (float)cfg.height;

    SDL_CreateWindow(ConvertString<String, std::string>(cfg.title).c_str(), cfg.width, cfg.height, SDL_WINDOW_OPENGL);
}

sgl::Window::~Window()
{
    SDL_DestroyWindow(mWindow);
    SDL_GL_DestroyContext(mGLContext);
    SDL_Quit();
}