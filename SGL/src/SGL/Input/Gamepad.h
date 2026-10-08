#pragma once
#include <SGL/SGL.h>
#include <SGL/Input/GamepadButton.h>
#include <SGL/Input/GamepadAxis.h>
#include <SGL/Window.h>
#include <SDL3/SDL.h>

SGL_BEGIN

/// @brief A helper for reading input from a gamepad
typedef struct sgl_Gamepad sgl_sealed
{
    SDL_Gamepad* handle;
    float deadzone; // defaults to 0.15

    bool buttonsThisFrame[(size_t)sgl_GamepadButton_COUNT];
    bool buttonsLastFrame[(size_t)sgl_GamepadButton_COUNT];

    float axesThisFrame[(size_t)sgl_GamepadAxis_COUNT];
    float axesLastFrame[(size_t)sgl_GamepadAxis_COUNT];

    sgl_Window* window;
} sgl_Gamepad;

/// @brief Create a new gamepad device
/// @param window The window to use
SGL_API extern sgl_Gamepad* sgl_Gamepad_New(sgl_Window* window);

/// @brief Destroy a gamepad
/// @param gp The gamepad to destroy
SGL_API extern void sgl_Gamepad_Destroy(sgl_Gamepad* gp);

/// @brief Update the internal state of a gamepad. Call once per frame.
/// Reconnects to the first available gamepad-capable device if none is
/// currently open, and drops the handle if it becomes invalid
/// @param gp The gamepad to update
SGL_API extern void sgl_Gamepad_Update(sgl_Gamepad* gp);

/// @brief Check if a gamepad is currently connected and open
/// @param gp The gamepad to check
SGL_API extern bool sgl_Gamepad_IsConnected(sgl_Gamepad* gp);

/// @brief Check if a button is down this frame
/// @param gp The gamepad to use
/// @param button The button to check
SGL_API extern bool sgl_Gamepad_IsButtonDown(sgl_Gamepad* gp, sgl_GamepadButton button);

/// @brief Check if a button is up this frame
/// @param gp The gamepad to use
/// @param button The button to check
SGL_API extern bool sgl_Gamepad_IsButtonUp(sgl_Gamepad* gp, sgl_GamepadButton button);

/// @brief Check if a button was just pressed this frame (down this frame, up last frame)
/// @param gp The gamepad to use
/// @param button The button to check
SGL_API extern bool sgl_Gamepad_IsButtonPressed(sgl_Gamepad* gp, sgl_GamepadButton button);

/// @brief Get the normalized value of an axis this frame. Sticks are deadzoned
/// and range [-1,1], triggers range [0,1]
/// @param gp The gamepad to use
/// @param axis The axis to check
SGL_API extern float sgl_Gamepad_GetAxis(sgl_Gamepad* gp, sgl_GamepadAxis axis);

/// @brief Get the normalized value of an axis last frame
/// @param gp The gamepad to use
/// @param axis The axis to check
SGL_API extern float sgl_Gamepad_GetAxisLastFrame(sgl_Gamepad* gp, sgl_GamepadAxis axis);

SGL_END
