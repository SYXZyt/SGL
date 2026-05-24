#pragma once
#include <SGL/SGL.h>
#include <SDL3/SDL.h>
#include <SGL/Maths/Vec2.h>

SGL_BEGIN

#define SGL_WINDOW_RESIZE_MAX_CALLBACK 32

typedef struct sgl_EngineConfig sgl_sealed
{
    const char* title;
    uint32 width;
    uint32 height;
    bool resizable;
} sgl_EngineConfig;

SGL_API extern const sgl_EngineConfig sgl_EngineConfig_Default;

typedef sglFuncPtr(sgl_Window_Resize_Callback, void, struct sgl_Window*);

typedef struct sgl_Window
{
    sgl_Window_Resize_Callback resizeCallbacks[SGL_WINDOW_RESIZE_MAX_CALLBACK];
    size_t resizeCallbackCount;

    sgl_EngineConfig cfg;
    sgl_Vec2i screenSize;
    sgl_Vec2i halfScreenSize;

    SDL_Window* window;
    SDL_GLContext glContext;

    bool wantsClose;

} sgl_Window;

SGL_API sgl_Window* sgl_Window_Create(sgl_EngineConfig cfg);
SGL_API extern void sgl_Window_Destroy(sgl_Window* window);

SGL_API extern void sgl_Window_PollEvents(sgl_Window* window);

SGL_END