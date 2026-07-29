#pragma once
#include <SGL/SGL.h>

SGL_BEGIN

/// @brief A button on the mouse
typedef uint8 sgl_MouseButton;
enum
{
    sgl_MouseButton_LEFT,
    sgl_MouseButton_RIGHT,
    sgl_MouseButton_MIDDLE,
    sgl_MouseButton_Size,
};

SGL_END