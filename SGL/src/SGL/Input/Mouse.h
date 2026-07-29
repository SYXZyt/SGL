#pragma once
#include <SGL/SGL.h>
#include <SGL/Input/MouseButton.h>
#include <SGL/Window.h>
#include <SGL/Maths/Vec2.h>

SGL_BEGIN

/// @brief A helper for reading mouse input
typedef struct sgl_Mouse sgl_sealed
{
    bool thisFrame[(size_t)sgl_MouseButton_Size];
    bool lastFrame[(size_t)sgl_MouseButton_Size];
    sgl_Window* window;

    sgl_Vec2 position;
    sgl_Vec2 delta;

    sgl_Vec2 scroll;
} sgl_Mouse;

/// @brief Create a new mouse device
/// @param window The window to use
SGL_API extern sgl_Mouse* sgl_Mouse_New(sgl_Window* window);

/// @brief Destroy a mouse device
/// @param mouse The mouse to destroy
SGL_API extern void sgl_Mouse_Destroy(sgl_Mouse* mouse);

/// @brief Update a mouse device (Call once per frame)
/// @param mouse The mouse to update
SGL_API extern void sgl_Mouse_Update(sgl_Mouse* mouse);

/// @brief Check if a mouse button is down this frame
/// @param mouse The mouse to use
/// @param btn The button to check
SGL_API extern bool sgl_Mouse_IsButtonDown(sgl_Mouse* mouse, sgl_MouseButton btn);

/// @brief Check if a mouse button is up this frame
/// @param mouse The mouse to use
/// @param btn The button to check
SGL_API extern bool sgl_Mouse_IsButtonUp(sgl_Mouse* mouse, sgl_MouseButton btn);

/// @brief Check if a mouse button was just pressed this frame
/// @param mouse The nouse to use
/// @param btn The button to check
/// @return True if down this frame and up the last
SGL_API extern bool sgl_Mouse_IsButtonPressed(sgl_Mouse* mouse, sgl_MouseButton btn);

/// @brief Get the position of a mouse cursor
/// @param mouse The mouse to use
SGL_API extern sgl_Vec2 sgl_Mouse_GetPosition(sgl_Mouse* mouse);

/// @brief Get the movement delta of a mouse cursor
/// @param mouse The mouse to use
SGL_API extern sgl_Vec2 sgl_Mouse_GetDelta(sgl_Mouse* mouse);

/// @brief Get the scroll wheel movement
/// @param mouse The mouse to use
SGL_API extern sgl_Vec2 sgl_Mouse_GetScroll(sgl_Mouse* mouse);

/// @brief Set if a mouse is using relative mode. This will hide the cursor
/// @param mouse The mouse to use
/// @param enabled True to enable, false to disable
SGL_API extern void sgl_Mouse_SetRelativeMode(sgl_Mouse* mouse, bool enabled);

/// @brief Check if a mouse is in relative mode
/// @param mouse The mouse to check
/// @return True if it is in relative mode
SGL_API extern bool sgl_Mouse_GetRelativeMode(sgl_Mouse* mouse);

SGL_END