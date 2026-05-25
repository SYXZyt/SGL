#include "Window.h"
#include <SGL/Util/Memory.h>
#include <SGL/Util/Error.h>
#include <vector>
#include <algorithm>
#include <glad/glad.h>

template <typename T>
static std::vector<T>& Void2Vec(void* ptr) {
    return *(std::vector<T>*)(ptr);
}

const sgl_EngineConfig sgl_EngineConfig_Default =
{
    .title = "SGL",
    .backend = sgl_Backend_OPENGL,
    .width = 1280,
    .height = 720,
    .resizable = true,
};

sgl_Window* sgl_Window_Create(sgl_EngineConfig cfg)
{
    sgl_Window* window = sgl::Memory::New<sgl_Window>();
    
    window->screenSize = window->halfScreenSize = sgl_Vec2i_Zero;
    window->window = nullptr;
    window->glContext = nullptr;
    window->wantsClose = false;
    window->cfg = cfg;
    window->callbacks = sgl::Memory::New<std::vector<sgl_Window_Resize_Callback>>();

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SGL_REPORT_ERROR("Failed to init SDL");
        return nullptr;
    }

    window->screenSize = {{{ (int)cfg.width, (int)cfg.height }}};
    window->halfScreenSize = sgl_Vec2i_Div_Scalar(window->screenSize, 2);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window->window = SDL_CreateWindow(cfg.title, cfg.width, cfg.height, (cfg.resizable ? SDL_WINDOW_RESIZABLE : 0) | SDL_WINDOW_OPENGL);

    if (cfg.backend == sgl_Backend_OPENGL)
    {
        window->glContext = SDL_GL_CreateContext(window->window);
        SDL_GL_MakeCurrent(window->window, window->glContext);
        gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
    }

    return window;
}

void sgl_Window_Destroy(sgl_Window* window)
{
    sgl::Memory::Delete((std::vector<sgl_Window_Resize_Callback>*)window->callbacks);

    if (window->cfg.backend == sgl_Backend_OPENGL)
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
            case SDL_EVENT_WINDOW_RESIZED:
            {
                sgl_Vec2i newSize{};
                newSize.width = event.window.data1;
                newSize.height = event.window.data2;

                window->screenSize = newSize;
                window->halfScreenSize = sgl_Vec2i_Div_Scalar(window->screenSize, 2);

                std::vector<sgl_Window_Resize_Callback>& callbacks = Void2Vec<sgl_Window_Resize_Callback>(window->callbacks);
                for (auto& [ptr, userdata] : callbacks)
                    ptr(window, newSize, userdata);

                break;
            }

            case SDL_EVENT_QUIT:
                window->wantsClose = true;
                break;
        }
    }
}

bool sgl_Window_WantClose(sgl_Window* window) {
    return window->wantsClose;
}

void sgl_Window_RegisterResize(sgl_Window* window, sgl_Window_Resize_Callback_ptr funcptr, void* userdata)
{
    std::vector<sgl_Window_Resize_Callback>& callbacks = Void2Vec<sgl_Window_Resize_Callback>(window->callbacks);

    // Check if this pointer is already registered
    // Not entirely sure what to do if it is. Should we update the userdata?
    auto it = std::find_if(callbacks.begin(), callbacks.end(), [funcptr](const sgl_Window_Resize_Callback& cb) { return cb.ptr == funcptr; });
    if (it != callbacks.end())
    {
        SGL_REPORT_ERROR("Callback already registered");
        return;
    }

    sgl_Window_Resize_Callback cb = {
        .ptr = funcptr,
        .userdata = userdata,
    };

    callbacks.push_back(cb);
}

void sgl_Window_DeregisterResize(sgl_Window* window, sgl_Window_Resize_Callback_ptr funcptr)
{
    std::vector<sgl_Window_Resize_Callback>& callbacks = Void2Vec<sgl_Window_Resize_Callback>(window->callbacks);
    auto it = std::find_if(callbacks.begin(), callbacks.end(), [funcptr](const sgl_Window_Resize_Callback& cb) { return cb.ptr == funcptr; });

    if (it != callbacks.end())
        callbacks.erase(it);
}

void sgl_Window_SwapBuffer(sgl_Window* window)
{
    if (window->glContext)
        SDL_GL_SwapWindow(window->window);
}