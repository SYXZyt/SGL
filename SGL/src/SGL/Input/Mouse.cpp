#include "Mouse.h"
#include <SDL3/SDL.h>
#include <SGL/Util/Memory.h>
#include <cstring>
#include <imgui.h>

sgl_Mouse* sgl_Mouse_New(sgl_Window* window)
{
    sgl_Mouse* mouse = sgl::Memory::New<sgl_Mouse>();
    mouse->window = window;

    std::memset(mouse->thisFrame, 0, sgl_MouseButton_Size);
    std::memset(mouse->lastFrame, 0, sgl_MouseButton_Size);

    mouse->position = mouse->delta = sgl_Vec2_Zero;
    mouse->scroll = sgl_Vec2_Zero;

    return mouse;
}

void sgl_Mouse_Destroy(sgl_Mouse* mouse) {
    sgl::Memory::Delete(mouse);
}

void sgl_Mouse_Update(sgl_Mouse* mouse)
{
    std::memcpy(mouse->lastFrame, mouse->thisFrame, sgl_MouseButton_Size);

    float x, y;
    SDL_MouseButtonFlags flags = SDL_GetMouseState(&x, &y);
    mouse->thisFrame[sgl_MouseButton_LEFT] = (flags & SDL_BUTTON_LMASK) != 0;
    mouse->thisFrame[sgl_MouseButton_RIGHT] = (flags & SDL_BUTTON_RMASK) != 0;
    mouse->thisFrame[sgl_MouseButton_MIDDLE] = (flags & SDL_BUTTON_MMASK) != 0;

    mouse->position = sgl_Vec2_New_ScalarXY(x, y);

    float dx, dy;
    SDL_GetRelativeMouseState(&dx, &dy);
    mouse->delta = sgl_Vec2_New_ScalarXY(dx, dy);

    // We can't sample scroll directly since it comes from a window event
    mouse->scroll = mouse->window->pendingScroll;
    mouse->window->pendingScroll = sgl_Vec2_Zero;
}

bool sgl_Mouse_IsButtonDown(sgl_Mouse* mouse, sgl_MouseButton btn)
{
    if (mouse->window->cfg.enableImGui && ImGui::GetIO().WantCaptureMouse)
        return false;

    return mouse->thisFrame[btn];
}

bool sgl_Mouse_IsButtonUp(sgl_Mouse* mouse, sgl_MouseButton btn)
{
    if (mouse->window->cfg.enableImGui && ImGui::GetIO().WantCaptureMouse)
        return true;

    return !mouse->thisFrame[btn];
}

bool sgl_Mouse_IsButtonPressed(sgl_Mouse* mouse, sgl_MouseButton btn)
{
    if (mouse->window->cfg.enableImGui && ImGui::GetIO().WantCaptureMouse)
        return false;

    return mouse->thisFrame[btn] && !mouse->lastFrame[btn];
}

sgl_Vec2 sgl_Mouse_GetPosition(sgl_Mouse* mouse) {
    return mouse->position;
}

sgl_Vec2 sgl_Mouse_GetDelta(sgl_Mouse* mouse) {
    return mouse->delta;
}

sgl_Vec2 sgl_Mouse_GetScroll(sgl_Mouse* mouse)
{
    if (mouse->window->cfg.enableImGui && ImGui::GetIO().WantCaptureMouse)
        return sgl_Vec2_Zero;

    return mouse->scroll;
}

void sgl_Mouse_SetRelativeMode(sgl_Mouse* mouse, bool enabled)
{
    SDL_SetWindowRelativeMouseMode(mouse->window->window, enabled);

    if (enabled)
    {
        // Discard whatever's accumulated so far so the first GetDelta() call
        // afterwards doesn't report a spurious jump.
        float dx, dy;
        SDL_GetRelativeMouseState(&dx, &dy);
    }
}

bool sgl_Mouse_GetRelativeMode(sgl_Mouse* mouse) {
    return SDL_GetWindowRelativeMouseMode(mouse->window->window);
}