#include "Gamepad.h"
#include <SDL3/SDL.h>
#include <SGL/Util/Memory.h>
#include <cstring>
#include <SGL/Maths/Maths.h>
#include <algorithm>

static float NormalizeAxis(sgl_GamepadAxis axis, Sint16 raw, float deadzone)
{
    bool isTrigger = axis == sgl_GamepadAxis_LEFT_TRIGGER || axis == sgl_GamepadAxis_RIGHT_TRIGGER;
    if (isTrigger)
        return sgl::Maths::Clamp(raw / 32767.0f, 0.0f, 1.0f);

    float v = sgl::Maths::Clamp(raw / 32768.0f, -1.0f, 1.0f);
    float mag = sgl::Maths::Abs(v);
    if (mag < deadzone)
        return 0.0f;

    float sign = v < 0.0f ? -1.0f : 1.0f;
    return sign * (mag - deadzone) / (1.0f - deadzone);
}

static void TryOpenFirstGamepad(sgl_Gamepad* gp)
{
    int count = 0;
    SDL_JoystickID* ids = SDL_GetGamepads(&count);
    if (!ids)
        return;

    for (int i = 0; i < count; ++i)
    {
        if (SDL_IsGamepad(ids[i]))
        {
            gp->handle = SDL_OpenGamepad(ids[i]);
            if (gp->handle)
                break;
        }
    }

    SDL_free(ids);
}

sgl_Gamepad* sgl_Gamepad_New(sgl_Window* window)
{
    sgl_Gamepad* gp = sgl::Memory::New<sgl_Gamepad>();
    gp->handle = nullptr;
    gp->window = window;
    gp->deadzone = 0.15f;

    std::memset(gp->buttonsThisFrame, 0, sizeof(gp->buttonsThisFrame));
    std::memset(gp->buttonsLastFrame, 0, sizeof(gp->buttonsLastFrame));
    std::memset(gp->axesThisFrame, 0, sizeof(gp->axesThisFrame));
    std::memset(gp->axesLastFrame, 0, sizeof(gp->axesLastFrame));

    return gp;
}

void sgl_Gamepad_Destroy(sgl_Gamepad* gp)
{
    if (gp->handle)
        SDL_CloseGamepad(gp->handle);

    sgl::Memory::Delete(gp);
}

void sgl_Gamepad_Update(sgl_Gamepad* gp)
{
    if (gp->handle && !SDL_GamepadConnected(gp->handle))
    {
        SDL_CloseGamepad(gp->handle);
        gp->handle = nullptr;
    }

    if (!gp->handle)
        TryOpenFirstGamepad(gp);

    std::memcpy(gp->buttonsLastFrame, gp->buttonsThisFrame, sizeof(gp->buttonsThisFrame));
    std::memcpy(gp->axesLastFrame, gp->axesThisFrame, sizeof(gp->axesThisFrame));

    for (int i = 0; i < sgl_GamepadButton_COUNT; ++i)
        gp->buttonsThisFrame[i] = gp->handle ? SDL_GetGamepadButton(gp->handle, (SDL_GamepadButton)i) : false;

    for (int i = 0; i < sgl_GamepadAxis_COUNT; ++i)
        gp->axesThisFrame[i] = gp->handle ? NormalizeAxis((sgl_GamepadAxis)i, SDL_GetGamepadAxis(gp->handle, (SDL_GamepadAxis)i), gp->deadzone) : 0.0f;
}

bool sgl_Gamepad_IsConnected(sgl_Gamepad* gp) {
    return gp->handle != nullptr;
}

bool sgl_Gamepad_IsButtonDown(sgl_Gamepad* gp, sgl_GamepadButton button) {
    return gp->buttonsThisFrame[button];
}

bool sgl_Gamepad_IsButtonUp(sgl_Gamepad* gp, sgl_GamepadButton button) {
    return !gp->buttonsThisFrame[button];
}

bool sgl_Gamepad_IsButtonPressed(sgl_Gamepad* gp, sgl_GamepadButton button) {
    return gp->buttonsThisFrame[button] && !gp->buttonsLastFrame[button];
}

float sgl_Gamepad_GetAxis(sgl_Gamepad* gp, sgl_GamepadAxis axis) {
    return gp->axesThisFrame[axis];
}

float sgl_Gamepad_GetAxisLastFrame(sgl_Gamepad* gp, sgl_GamepadAxis axis) {
    return gp->axesLastFrame[axis];
}
