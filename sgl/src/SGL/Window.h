#pragma once
#include <SGL/SGL.h>
#include <SDL3/SDL.h>
#include <SGL/Maths/Vec2.h>

SGL_BEGIN

typedef struct sgl_EngineConfig sgl_sealed
{
    const char* title;
    uint32 width;
    uint32 height;
    bool resizable;
} sgl_EngineConfig;

SGL_API extern const sgl_EngineConfig sgl_EngineConfig_Default;

struct sgl_Window;

typedef sglFuncPtr(sgl_Window_Resize_Callback_ptr, void, struct sgl_Window*, sgl_Vec2i, void*);

typedef struct sgl_Window_Resize_Callback sgl_sealed
{
    sgl_Window_Resize_Callback_ptr ptr;
    void* userdata;
} sgl_Window_Resize_Callback;

typedef struct sgl_Window
{
    sgl_EngineConfig cfg;
    sgl_Vec2i screenSize;
    sgl_Vec2i halfScreenSize;

    SDL_Window* window;
    SDL_GLContext glContext;

    void* callbacks; // std::vector<sgl_Window_Resize_Callback>

    bool wantsClose;

} sgl_Window;

SGL_API sgl_Window* sgl_Window_Create(sgl_EngineConfig cfg);
SGL_API extern void sgl_Window_Destroy(sgl_Window* window);

SGL_API extern void sgl_Window_PollEvents(sgl_Window* window);
SGL_API extern bool sgl_Window_WantClose(sgl_Window* window);

SGL_API extern void sgl_Window_RegisterResize(sgl_Window* window, sgl_Window_Resize_Callback_ptr funcptr, void* userdata);
SGL_API extern void sgl_Window_DeregisterResize(sgl_Window* window, sgl_Window_Resize_Callback_ptr funcptr);

SGL_END