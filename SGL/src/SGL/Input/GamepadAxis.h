#pragma once
#include <SGL/SGL.h>

SGL_BEGIN

typedef int sgl_GamepadAxis;
enum
{
    sgl_GamepadAxis_LEFTX,
    sgl_GamepadAxis_LEFTY,
    sgl_GamepadAxis_RIGHTX,
    sgl_GamepadAxis_RIGHTY,
    sgl_GamepadAxis_LEFT_TRIGGER,
    sgl_GamepadAxis_RIGHT_TRIGGER,
    sgl_GamepadAxis_COUNT,
};

SGL_END
