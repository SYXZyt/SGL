#include "Window.h"
#include <sgl/Error.h>
#include <glad/glad.h>
#include <sgl/Util/Logger.h>

const sgl::Window::Config& sgl::Window::Config::Default = {};

void sgl::Window::PollEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_EVENT_WINDOW_RESIZED:
            {
                mScreenSize = { event.window.data1, event.window.data2 };
                mHalfScreenSize = mScreenSize / 2;

                OnResize.Invoke(mScreenSize);

                break;
            }
            case SDL_EVENT_QUIT:
                mWantClose = true;
                break;
        }
    }
}

sgl::Window::Window(const Config& cfg) :
    mWindow(nullptr)
#ifdef _WIN32
    , mHwnd(nullptr)
#endif
{
#ifndef SGL_DIRECTX
    if (cfg.graphicsApi == GraphicsAPI::DIRECTX11)
    {
        SGL_REPORT_ERROR(u8"Directx is not supported on this build");
        abort();
    }
#endif

    Logger::Init();

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SGL_REPORT_ERROR(u8"Failed to init sdl");
        return;
    }

    mScreenSize = { (int)cfg.width, (int)cfg.height };
    mHalfScreenSize = mScreenSize / 2;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    mWindow = SDL_CreateWindow(ConvertString<String, std::string>(cfg.title).c_str(), cfg.width, cfg.height, SDL_WINDOW_OPENGL | (cfg.resizable ? SDL_WINDOW_RESIZABLE : 0));

#ifdef _WIN32
    SDL_PropertiesID props = SDL_GetWindowProperties(mWindow);
    mHwnd = (HWND)SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
#endif

    if (cfg.graphicsApi == GraphicsAPI::OPENGL)
    {
        mGLContext = SDL_GL_CreateContext(mWindow);
        SDL_GL_MakeCurrent(mWindow, mGLContext);
        gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    }
}

sgl::Window::~Window()
{
    SDL_GL_DestroyContext(mGLContext);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();

    Logger::Shutdown();
}