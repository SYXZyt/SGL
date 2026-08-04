#pragma once
#include <SGL/SGL.h>
#include <SGL/Input/Key.h>
#include <SGL/Window.h>

SGL_BEGIN

/// @brief A helper for reading keyboard input
typedef struct sgl_Keyboard sgl_sealed
{
    bool thisFrame[(size_t)sgl_Key_COUNT];
    bool lastFrame[(size_t)sgl_Key_COUNT];
    sgl_Window* window;
} sgl_Keyboard;

/// @brief Create a new keyboard device
/// @param window The window to use
SGL_API extern sgl_Keyboard* sgl_Keyboard_New(sgl_Window* window);

/// @brief Destroy a keyboard
/// @param kb The keyboard to destroy
SGL_API extern void sgl_Keyboard_Destroy(sgl_Keyboard* kb);

/// @brief Update the internal state of a keyboad. Call once per frame
/// @param kb The keyboard to update
SGL_API extern void sgl_Keyboard_Update(sgl_Keyboard* kb);

/// @brief Check if a key is down this frame
/// @param kb The keyboard to use
/// @param key The key to check
SGL_API extern bool sgl_Keyboard_IsKeyDown(sgl_Keyboard* kb, sgl_Key key);

/// @brief Check if a key is up this frame
/// @param kb The keyboard to use
/// @param key The key to check
SGL_API extern bool sgl_Keyboard_IsKeyUp(sgl_Keyboard* kb, sgl_Key key);

/// @brief Check if a key is just pressed this frame (down this frame, up last frame)
/// @param kb The keyboard to use
/// @param key The key to check
SGL_API extern bool sgl_Keyboard_IsKeyPressed(sgl_Keyboard* kb, sgl_Key key);

SGL_END