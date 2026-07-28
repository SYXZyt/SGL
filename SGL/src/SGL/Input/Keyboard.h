#pragma once
#include <SGL/SGL.h>
#include <SGL/Input/Key.h>
#include <SGL/Window.h>

SGL_BEGIN

typedef struct sgl_Keyboard sgl_sealed
{
    bool thisFrame[(size_t)sgl_Key_COUNT];
    bool lastFrame[(size_t)sgl_Key_COUNT];
    sgl_Window* window;
} sgl_Keyboard;

SGL_API extern sgl_Keyboard* sgl_Keyboard_New(sgl_Window* window);
SGL_API extern void sgl_Keyboard_Destroy(sgl_Keyboard* kb);

SGL_API extern void sgl_Keyboard_Update(sgl_Keyboard* kb);

SGL_API extern bool sgl_Keyboard_IsKeyDown(sgl_Keyboard* kb, sgl_Key key);
SGL_API extern bool sgl_Keyboard_IsKeyUp(sgl_Keyboard* kb, sgl_Key key);
SGL_API extern bool sgl_Keyboard_IsKeyPressed(sgl_Keyboard* kb, sgl_Key key);

SGL_END