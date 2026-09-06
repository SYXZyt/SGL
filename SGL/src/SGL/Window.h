#pragma once
#include <SGL/SGL.h>
#include <SDL3/SDL.h>
#include <SGL/Maths/Vec2.h>

SGL_BEGIN

/// @brief Supported graphics APIs
typedef uint8 sgl_Backend;
enum
{
    sgl_Backend_OPENGL,
    sgl_Backend_DIRECTX11,
};

/// @brief Configure the SGL engine
typedef struct sgl_EngineConfig sgl_sealed
{
    const char* title;
    sgl_Backend backend;
    uint32 width;
    uint32 height;
    SDL_InitFlags sdlFlags;
    bool resizable;
    bool enableImGui;
} sgl_EngineConfig;

/// @brief Default configuration
SGL_API extern const sgl_EngineConfig sgl_EngineConfig_Default;

struct sgl_Window;

typedef sglFuncPtr(sgl_Window_Resize_Callback_ptr, void, struct sgl_Window*, sgl_Vec2i, void*);

typedef struct sgl_Window_Resize_Callback sgl_sealed
{
    sgl_Window_Resize_Callback_ptr ptr;
    void* userdata;
} sgl_Window_Resize_Callback;

/// @brief Handles the window which will be rendered to
typedef struct sgl_Window
{
    sgl_EngineConfig cfg;
    sgl_Vec2i screenSize;
    sgl_Vec2i halfScreenSize;

    SDL_Window* window;
    SDL_GLContext glContext;

    void* callbacks; // std::vector<sgl_Window_Resize_Callback>
    sgl_Vec2 pendingScroll;

    bool wantsClose;

} sgl_Window;

/// @brief Create a new window, wrapping SDL3
/// @param cfg The engine config
SGL_API sgl_Window* sgl_Window_Create(sgl_EngineConfig cfg);

/// @brief Destroy a window
/// @param window The window to destroy
SGL_API extern void sgl_Window_Destroy(sgl_Window* window);

/// @brief Poll SDL events
/// @param window The window to use
SGL_API extern void sgl_Window_PollEvents(sgl_Window* window);

/// @brief Check if the window is still open
/// @param window The window to check
/// @return True if the window wants to close (ALT F4, X button, etc)
SGL_API extern bool sgl_Window_WantClose(sgl_Window* window);

/// @brief Register a resize callback
/// @param window The window to add the callback to
/// @param funcptr The callback to register
/// @param userdata Any extra data the callback may need
SGL_API extern void sgl_Window_RegisterResize(sgl_Window* window, sgl_Window_Resize_Callback_ptr funcptr, void* userdata);

/// @brief Deregister a resize callbacl
/// @param window The window to remove the callback from
/// @param funcptr The callback to remove
SGL_API extern void sgl_Window_DeregisterResize(sgl_Window* window, sgl_Window_Resize_Callback_ptr funcptr);

/// @brief Swap the internal window buffer
/// @param window The window to use
SGL_API extern void sgl_Window_SwapBuffer(sgl_Window* window);

#ifdef _WIN32
/// @brief Windows Only. Get the WIN32 window HWND
/// @param window The window to get the HWND of
SGL_API extern HWND sgl_Window_GetWin32Window(sgl_Window* window);
#endif

SGL_END