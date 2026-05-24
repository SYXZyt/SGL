#include "Window.h"
#include <SGL/Util/Memory.h>
#include <SGL/Util/Error.h>

const sgl_EngineConfig sgl_EngineConfig_Default =
{
    .title = "SGL",
    .width = 1280,
    .height = 720,
    .resizable = true,
};

sgl_Window* sgl_Window_Create(sgl_EngineConfig cfg)
{
    sgl_Window* window = sgl::Memory::New<sgl_Window>();
    
    window->resizeCallbackCount = 0;
    window->screenSize = window->halfScreenSize = sgl_Vec2i_Zero;
    window->window = nullptr;
    window->glContext = nullptr;
    window->wantsClose = false;
    window->cfg = cfg;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SGL_REPORT_ERROR("Failed to init SDL");
        return nullptr;
    }

    window->screenSize = { (int)cfg.width, (int)cfg.height };
    window->halfScreenSize = sgl_Vec2i_Div_Scalar(window->screenSize, 2);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window->window = SDL_CreateWindow(cfg.title, cfg.width, cfg.height, (cfg.resizable ? SDL_WINDOW_RESIZABLE : 0));

    window->glContext = SDL_GL_CreateContext(window->window);
    SDL_GL_MakeCurrent(window->window, window->glContext);

    return window;
}

void sgl_Window_Destroy(sgl_Window* window)
{
    SDL_GL_DestroyContext(window->glContext);
    SDL_DestroyWindow(window->window);
    SDL_Quit();

    sgl::Memory::Delete(window);
}

void sgl_Window_PollEvents(sgl_Window* window)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_EVENT_QUIT:
                window->wantsClose = true;
                break;
        }
    }
}