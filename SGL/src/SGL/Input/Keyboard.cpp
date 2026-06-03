#include "Keyboard.h"
#include <SDL3/SDL.h>
#include <SGL/Util/Memory.h>
#include <cstring>
#include <imgui.h>

sgl_Keyboard* sgl_Keyboard_New()
{
    sgl_Keyboard* kb = sgl::Memory::New<sgl_Keyboard>();

    std::memset(kb->thisFrame, 0, sgl_Key_COUNT);
    std::memset(kb->lastFrame, 0, sgl_Key_COUNT);

    return kb;
}

void sgl_Keyboard_Destroy(sgl_Keyboard* kb) {
    sgl::Memory::Delete(kb);
}

void sgl_Keyboard_Update(sgl_Keyboard* kb)
{
    std::memcpy(kb->lastFrame, kb->thisFrame, sgl_Key_COUNT);

    const bool* keys = (const bool*)SDL_GetKeyboardState(nullptr);
    std::memcpy(kb->thisFrame, keys, sgl_Key_COUNT);
}

bool sgl_Keyboard_IsKeyDown(sgl_Keyboard* kb, sgl_Key key)
{
    if (ImGui::GetIO().WantCaptureKeyboard)
        return false;

    return kb->thisFrame[key];
}

bool sgl_Keyboard_IsKeyUp(sgl_Keyboard* kb, sgl_Key key)
{
    if (ImGui::GetIO().WantCaptureKeyboard)
        return true;

    return !kb->thisFrame[key];
}

bool sgl_Keyboard_IsKeyPressed(sgl_Keyboard* kb, sgl_Key key)
{
    if (ImGui::GetIO().WantCaptureKeyboard)
        return false;

    return kb->thisFrame[key] && !kb->lastFrame[key];
}