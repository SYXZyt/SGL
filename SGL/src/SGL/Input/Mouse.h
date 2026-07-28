#pragma once
#include <SGL/SGL.h>
#include <SGL/Input/MouseButton.h>
#include <SGL/Window.h>
#include <SGL/Maths/Vec2.h>

SGL_BEGIN

typedef struct sgl_Mouse sgl_sealed
{
    bool thisFrame[(size_t)sgl_MouseButton_Size];
    bool lastFrame[(size_t)sgl_MouseButton_Size];
    sgl_Window* window;

    sgl_Vec2 position;
    sgl_Vec2 delta;

    sgl_Vec2 scroll;
} sgl_Mouse;

SGL_API extern sgl_Mouse* sgl_Mouse_New(sgl_Window* window);
SGL_API extern void sgl_Mouse_Destroy(sgl_Mouse* mouse);

SGL_API extern void sgl_Mouse_Update(sgl_Mouse* mouse);

SGL_API extern bool sgl_Mouse_IsButtonDown(sgl_Mouse* mouse, sgl_MouseButton btn);
SGL_API extern bool sgl_Mouse_IsButtonUp(sgl_Mouse* mouse, sgl_MouseButton btn);
SGL_API extern bool sgl_Mouse_IsButtonPressed(sgl_Mouse* mouse, sgl_MouseButton btn);

SGL_API extern sgl_Vec2 sgl_Mouse_GetPosition(sgl_Mouse* mouse);
SGL_API extern sgl_Vec2 sgl_Mouse_GetDelta(sgl_Mouse* mouse);
SGL_API extern sgl_Vec2 sgl_Mouse_GetScroll(sgl_Mouse* mouse);

SGL_API extern void sgl_Mouse_SetRelativeMode(sgl_Mouse* mouse, bool enabled);
SGL_API extern bool sgl_Mouse_GetRelativeMode(sgl_Mouse* mouse);

SGL_END