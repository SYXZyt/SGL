#include "ImGui.h"
#include <imgui.h>
#include <cstring>

static ImVec2* SglToIm(sgl_Vec2* v) {
    return reinterpret_cast<ImVec2*>(v);
}

static ImColor* SglToIm(sgl_Colour* c) {
    return reinterpret_cast<ImColor*>(c);
}

static sgl_Vec2* ImToSgl(ImVec2* v) {
    return reinterpret_cast<sgl_Vec2*>(v);
}

static sgl_Colour* ImToSgl(ImVec4* c) {
    return reinterpret_cast<sgl_Colour*>(c);
}

static float* GetStyleFloat(ImGuiStyle& style, sgl_StyleVar var)
{
    switch (var)
    {
        case sgl_ImGuiStyleVar_Alpha:
            return &style.Alpha;

        case sgl_ImGuiStyleVar_DisabledAlpha:
            return &style.DisabledAlpha;

        case sgl_ImGuiStyleVar_WindowRounding:
            return &style.WindowRounding;

        case sgl_ImGuiStyleVar_WindowBorderSize:
            return &style.WindowBorderSize;

        case sgl_ImGuiStyleVar_ChildRounding:
            return &style.ChildRounding;

        case sgl_ImGuiStyleVar_ChildBorderSize:
            return &style.ChildBorderSize;

        case sgl_ImGuiStyleVar_PopupRounding:
            return &style.PopupRounding;

        case sgl_ImGuiStyleVar_PopupBorderSize:
            return &style.PopupBorderSize;

        case sgl_ImGuiStyleVar_FrameRounding:
            return &style.FrameRounding;

        case sgl_ImGuiStyleVar_FrameBorderSize:
            return &style.FrameBorderSize;

        case sgl_ImGuiStyleVar_IndentSpacing:
            return &style.IndentSpacing;

        case sgl_ImGuiStyleVar_ScrollbarSize:
            return &style.ScrollbarSize;

        case sgl_ImGuiStyleVar_ScrollbarRounding:
            return &style.ScrollbarRounding;

        case sgl_ImGuiStyleVar_ScrollbarPadding:
            return &style.ScrollbarPadding;

        case sgl_ImGuiStyleVar_GrabMinSize:
            return &style.GrabMinSize;

        case sgl_ImGuiStyleVar_GrabRounding:
            return &style.GrabRounding;

        case sgl_ImGuiStyleVar_ImageRounding:
            return &style.ImageRounding;

        case sgl_ImGuiStyleVar_ImageBorderSize:
            return &style.ImageBorderSize;

        case sgl_ImGuiStyleVar_TabRounding:
            return &style.TabRounding;

        case sgl_ImGuiStyleVar_TabBorderSize:
            return &style.TabBorderSize;

        case sgl_ImGuiStyleVar_TabMinWidthBase:
            return &style.TabMinWidthBase;

        case sgl_ImGuiStyleVar_TabMinWidthShrink:
            return &style.TabMinWidthShrink;

        case sgl_ImGuiStyleVar_TabBarBorderSize:
            return &style.TabBarBorderSize;

        case sgl_ImGuiStyleVar_TabBarOverlineSize:
            return &style.TabBarOverlineSize;

        case sgl_ImGuiStyleVar_TableAngledHeadersAngle:
            return &style.TableAngledHeadersAngle;

        case sgl_ImGuiStyleVar_TreeLinesSize:
            return &style.TreeLinesSize;

        case sgl_ImGuiStyleVar_TreeLinesRounding:
            return &style.TreeLinesRounding;

        case sgl_ImGuiStyleVar_SeparatorSize:
            return &style.SeparatorSize;

        case sgl_ImGuiStyleVar_SeparatorTextBorderSize:
            return &style.SeparatorTextBorderSize;

        default:
            return nullptr;
    }
}

static sgl_Vec2* GetStyleVec2(ImGuiStyle& style, sgl_StyleVar var)
{
    switch (var)
    {
        case sgl_ImGuiStyleVar_WindowPadding:
            return ImToSgl(&style.WindowPadding);

        case sgl_ImGuiStyleVar_WindowMinSize:
            return ImToSgl(&style.WindowMinSize);

        case sgl_ImGuiStyleVar_WindowTitleAlign:
            return ImToSgl(&style.WindowTitleAlign);

        case sgl_ImGuiStyleVar_FramePadding:
            return ImToSgl(&style.FramePadding);

        case sgl_ImGuiStyleVar_ItemSpacing:
            return ImToSgl(&style.ItemSpacing);

        case sgl_ImGuiStyleVar_ItemInnerSpacing:
            return ImToSgl(&style.ItemInnerSpacing);

        case sgl_ImGuiStyleVar_CellPadding:
            return ImToSgl(&style.CellPadding);

        case sgl_ImGuiStyleVar_TableAngledHeadersTextAlign:
            return ImToSgl(&style.TableAngledHeadersTextAlign);

        case sgl_ImGuiStyleVar_ButtonTextAlign:
            return ImToSgl(&style.ButtonTextAlign);

        case sgl_ImGuiStyleVar_SelectableTextAlign:
            return ImToSgl(&style.SelectableTextAlign);

        case sgl_ImGuiStyleVar_SeparatorTextAlign:
            return ImToSgl(&style.SeparatorTextAlign);

        case sgl_ImGuiStyleVar_SeparatorTextPadding:
            return ImToSgl(&style.SeparatorTextPadding);

        default:
            return nullptr;
    }
}

bool sgl_Begin(const char* title, bool* open, sgl_WindowFlags flags) {
    return ImGui::Begin(title, open, flags);
}

void sgl_End() {
    ImGui::End();
}

void sgl_Separator() {
    ImGui::Separator();
}

void sgl_SeparatorText(const char* text) {
    ImGui::SeparatorText(text);
}

void sgl_BeginDisabled() {
    ImGui::BeginDisabled();
}

void sgl_EndDisabled() {
    ImGui::EndDisabled();
}

void sgl_SameLine() {
    ImGui::SameLine();
}

void sgl_SameLineEx(float offsetFromStart, float spacing) {
    ImGui::SameLine(offsetFromStart, spacing);
}

void sgl_PushID(int id) {
    ImGui::PushID(id);
}

void sgl_PopID() {
    ImGui::PopID();
}

void sgl_SetStyleVar_float(sgl_StyleVar style, float val)
{
    ImGuiStyle& imStyle = ImGui::GetStyle();
    float* styleValue = GetStyleFloat(imStyle, style);
    if (styleValue)
        *styleValue = val;
}

void sgl_SetStyleVar_Vec2(sgl_StyleVar style, sgl_Vec2 val)
{
    ImGuiStyle& imStyle = ImGui::GetStyle();
    sgl_Vec2* styleValue = GetStyleVec2(imStyle, style);
    if (styleValue)
        *styleValue = val;
}

void sgl_SetStyleVar_Colour(sgl_StyleCol style, sgl_Colour col)
{
    ImGuiStyle& imStyle = ImGui::GetStyle();
    *ImToSgl(&imStyle.Colors[style]) = col;
}

void sgl_PushStyleVar_float(sgl_StyleVar style, float val) {
    ImGui::PushStyleVar(style, val);
}

void sgl_PushStyleVar_Vec2(sgl_StyleVar style, sgl_Vec2 val)
{
    ImVec2* v = SglToIm(&val);
    ImGui::PushStyleVar(style, *v);
}

void sgl_PopStyleVar() {
    ImGui::PopStyleVar();
}

bool sgl_Button(const char* text) {
    return ImGui::Button(text);
}

bool sgl_ButtonEx(const char* text, sgl_Vec2 size)
{
    ImVec2* v = SglToIm(&size);
    return ImGui::Button(text, *v);
}

void sgl_Text(const char* fmt, ...)
{
    va_list va;
    va_start(va, fmt);

    ImGui::TextV(fmt, va);

    va_end(va);
}

void sgl_TextUnformatted(const char* text) {
    ImGui::TextUnformatted(text);
}

void sgl_TextColoured(sgl_Colour colour, const char* fmt, ...)
{
    ImColor* c = SglToIm(&colour);

    va_list va;
    va_start(va, fmt);

    ImGui::TextColoredV(*c, fmt, va);

    va_end(va);
}

void sgl_TextUnformattedColoured(sgl_Colour colour, const char* text)
{
    ImColor* c = SglToIm(&colour);
    ImGui::TextColored(*c, "%s", text);
}

bool sgl_BeginMainMenuBar() {
    return ImGui::BeginMainMenuBar();
}

void sgl_EndMainMenuBar() {
    ImGui::EndMainMenuBar();
}

bool sgl_BeginMenu(const char* text, bool enabled) {
    return ImGui::BeginMenu(text, enabled);
}

void sgl_EndMenu() {
    ImGui::EndMenu();
}

bool sgl_MenuItem(const char* text, const char* shortcut, bool selected, bool enabled) {
    return ImGui::MenuItem(text, shortcut, selected, enabled);
}

sgl_Vec2 sgl_GetContentRegionAvail()
{
    ImVec2 v = ImGui::GetContentRegionAvail();
    return {{{ v.x, v.y }}};
}

sgl_Vec2 sgl_CalcTextSize(const char* text, const char* textEnd, bool hideTextAfterDoubleHash, float wrapWidth)
{
    ImVec2 v = ImGui::CalcTextSize(text, textEnd, hideTextAfterDoubleHash, wrapWidth);
    return {{{ v.x, v.y }}};
}

bool sgl_InputText(const char* text, char* buffer, size_t bufferSize, sgl_InputTextFlags flags) {
    return ImGui::InputText(text, buffer, bufferSize, flags);
}

bool sgl_InputInt(const char* text, int* v, int step, int stepFast, sgl_InputTextFlags flags) {
    return ImGui::InputInt(text, v, step, stepFast, flags);
}

bool sgl_InputFloat(const char* text, float* v, float step, float stepFast, sgl_InputTextFlags flags) {
    return ImGui::InputFloat(text, v, step, stepFast, "%.3f", flags);
}

bool sgl_InputVec2(const char* text, sgl_Vec2* v, sgl_InputTextFlags flags) {
    return ImGui::InputFloat2(text, &v->x, "%.3f", flags);
}

bool sgl_SliderFloat(const char* text, float* v, float min, float max) {
    return ImGui::SliderFloat(text, v, min, max);
}

bool sgl_SliderAngle(const char* text, float* v, float min, float max) {
    return ImGui::SliderAngle(text, v, min, max);
}

bool sgl_SliderInt(const char* text, int* v, int min, int max) {
    return ImGui::SliderInt(text, v, min, max);
}

bool sgl_DragFloat(const char* text, float* v, float speed, float min, float max) {
    return ImGui::DragFloat(text, v, speed, min, max);
}

bool sgl_DragInt(const char* text, int* v, float speed, int min, int max) {
    return ImGui::DragInt(text, v, speed, min, max);
}

bool sgl_DragVec2(const char* text, sgl_Vec2* v, float speed, float min, float max) {
    return ImGui::DragFloat2(text, &v->x, speed, min, max);
}

bool sgl_Checkbox(const char* text, bool* v) {
    return ImGui::Checkbox(text, v);
}

bool sgl_CollapsableHeader(const char* text) {
    return ImGui::CollapsingHeader(text);
}

bool sgl_TreeNode(const char* text) {
    return ImGui::TreeNode(text);
}

void sgl_TreePop() {
    ImGui::TreePop();
}