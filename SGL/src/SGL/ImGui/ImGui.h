#pragma once
#include <SGL/SGL.h>
#include <SGL/Maths/Vec2.h>
#include <SGL/Colour.h>

SGL_BEGIN

typedef enum sgl_WindowFlags
{
    sgl_ImGuiWindowFlags_None = 0,
    sgl_ImGuiWindowFlags_NoTitleBar = 1 << 0,   // Disable title-bar
    sgl_ImGuiWindowFlags_NoResize = 1 << 1,   // Disable user resizing with the lower-right grip
    sgl_ImGuiWindowFlags_NoMove = 1 << 2,   // Disable user moving the window
    sgl_ImGuiWindowFlags_NoScrollbar = 1 << 3,   // Disable scrollbars (window can still scroll with mouse or programmatically)
    sgl_ImGuiWindowFlags_NoScrollWithMouse = 1 << 4,   // Disable user vertically scrolling with mouse wheel. On child window, mouse wheel will be forwarded to the parent unless NoScrollbar is also set.
    sgl_ImGuiWindowFlags_NoCollapse = 1 << 5,   // Disable user collapsing window by double-clicking on it. Also referred to as Window Menu Button (e.g. within a docking node).
    sgl_ImGuiWindowFlags_AlwaysAutoResize = 1 << 6,   // Resize every window to its content every frame
    sgl_ImGuiWindowFlags_NoBackground = 1 << 7,   // Disable drawing background color (WindowBg, etc.) and outside border. Similar as using SetNextWindowBgAlpha(0.0f).
    sgl_ImGuiWindowFlags_NoSavedSettings = 1 << 8,   // Never load/save settings in .ini file
    sgl_ImGuiWindowFlags_NoMouseInputs = 1 << 9,   // Disable catching mouse, hovering test with pass through.
    sgl_ImGuiWindowFlags_MenuBar = 1 << 10,  // Has a menu-bar
    sgl_ImGuiWindowFlags_HorizontalScrollbar = 1 << 11,  // Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in ImGui_demo in the "Horizontal Scrolling" section.
    sgl_ImGuiWindowFlags_NoFocusOnAppearing = 1 << 12,  // Disable taking focus when transitioning from hidden to visible state
    sgl_ImGuiWindowFlags_NoBringToFrontOnFocus = 1 << 13,  // Disable bringing window to front when taking focus (e.g. clicking on it or programmatically giving it focus)
    sgl_ImGuiWindowFlags_AlwaysVerticalScrollbar = 1 << 14,  // Always show vertical scrollbar (even if ContentSize.y < Size.y)
    sgl_ImGuiWindowFlags_AlwaysHorizontalScrollbar = 1 << 15,  // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
    sgl_ImGuiWindowFlags_NoNavInputs = 1 << 16,  // No keyboard/gamepad navigation within the window
    sgl_ImGuiWindowFlags_NoNavFocus = 1 << 17,  // No focusing toward this window with keyboard/gamepad navigation (e.g. skipped by Ctrl+Tab)
    sgl_ImGuiWindowFlags_UnsavedDocument = 1 << 18,  // Display a dot next to the title. When used in a tab/docking context, tab is selected when clicking the X + closure is not assumed (will wait for user to stop submitting the tab). Otherwise closure is assumed when pressing the X, so if you keep submitting the tab may reappear at end of tab bar.
    sgl_ImGuiWindowFlags_NoNav = sgl_ImGuiWindowFlags_NoNavInputs | sgl_ImGuiWindowFlags_NoNavFocus,
    sgl_ImGuiWindowFlags_NoDecoration = sgl_ImGuiWindowFlags_NoTitleBar | sgl_ImGuiWindowFlags_NoResize | sgl_ImGuiWindowFlags_NoScrollbar | sgl_ImGuiWindowFlags_NoCollapse,
    sgl_ImGuiWindowFlags_NoInputs = sgl_ImGuiWindowFlags_NoMouseInputs | sgl_ImGuiWindowFlags_NoNavInputs | sgl_ImGuiWindowFlags_NoNavFocus,
} sgl_WindowFlags;

typedef enum sgl_StyleVar
{
    // Enum name -------------------------- // Member in ImGuiStyle structure (see ImGuiStyle for descriptions)
    sgl_ImGuiStyleVar_Alpha,                    // float     Alpha
    sgl_ImGuiStyleVar_DisabledAlpha,            // float     DisabledAlpha
    sgl_ImGuiStyleVar_WindowPadding,            // ImVec2    WindowPadding
    sgl_ImGuiStyleVar_WindowRounding,           // float     WindowRounding
    sgl_ImGuiStyleVar_WindowBorderSize,         // float     WindowBorderSize
    sgl_ImGuiStyleVar_WindowMinSize,            // ImVec2    WindowMinSize
    sgl_ImGuiStyleVar_WindowTitleAlign,         // ImVec2    WindowTitleAlign
    sgl_ImGuiStyleVar_ChildRounding,            // float     ChildRounding
    sgl_ImGuiStyleVar_ChildBorderSize,          // float     ChildBorderSize
    sgl_ImGuiStyleVar_PopupRounding,            // float     PopupRounding
    sgl_ImGuiStyleVar_PopupBorderSize,          // float     PopupBorderSize
    sgl_ImGuiStyleVar_FramePadding,             // ImVec2    FramePadding
    sgl_ImGuiStyleVar_FrameRounding,            // float     FrameRounding
    sgl_ImGuiStyleVar_FrameBorderSize,          // float     FrameBorderSize
    sgl_ImGuiStyleVar_ItemSpacing,              // ImVec2    ItemSpacing
    sgl_ImGuiStyleVar_ItemInnerSpacing,         // ImVec2    ItemInnerSpacing
    sgl_ImGuiStyleVar_IndentSpacing,            // float     IndentSpacing
    sgl_ImGuiStyleVar_CellPadding,              // ImVec2    CellPadding
    sgl_ImGuiStyleVar_ScrollbarSize,            // float     ScrollbarSize
    sgl_ImGuiStyleVar_ScrollbarRounding,        // float     ScrollbarRounding
    sgl_ImGuiStyleVar_ScrollbarPadding,         // float     ScrollbarPadding
    sgl_ImGuiStyleVar_GrabMinSize,              // float     GrabMinSize
    sgl_ImGuiStyleVar_GrabRounding,             // float     GrabRounding
    sgl_ImGuiStyleVar_ImageRounding,            // float     ImageRounding
    sgl_ImGuiStyleVar_ImageBorderSize,          // float     ImageBorderSize
    sgl_ImGuiStyleVar_TabRounding,              // float     TabRounding
    sgl_ImGuiStyleVar_TabBorderSize,            // float     TabBorderSize
    sgl_ImGuiStyleVar_TabMinWidthBase,          // float     TabMinWidthBase
    sgl_ImGuiStyleVar_TabMinWidthShrink,        // float     TabMinWidthShrink
    sgl_ImGuiStyleVar_TabBarBorderSize,         // float     TabBarBorderSize
    sgl_ImGuiStyleVar_TabBarOverlineSize,       // float     TabBarOverlineSize
    sgl_ImGuiStyleVar_TableAngledHeadersAngle,  // float     TableAngledHeadersAngle
    sgl_ImGuiStyleVar_TableAngledHeadersTextAlign,// ImVec2  TableAngledHeadersTextAlign
    sgl_ImGuiStyleVar_TreeLinesSize,            // float     TreeLinesSize
    sgl_ImGuiStyleVar_TreeLinesRounding,        // float     TreeLinesRounding
    sgl_ImGuiStyleVar_ButtonTextAlign,          // ImVec2    ButtonTextAlign
    sgl_ImGuiStyleVar_SelectableTextAlign,      // ImVec2    SelectableTextAlign
    sgl_ImGuiStyleVar_SeparatorSize,            // float     SeparatorSize
    sgl_ImGuiStyleVar_SeparatorTextBorderSize,  // float     SeparatorTextBorderSize
    sgl_ImGuiStyleVar_SeparatorTextAlign,       // ImVec2    SeparatorTextAlign
    sgl_ImGuiStyleVar_SeparatorTextPadding,     // ImVec2    SeparatorTextPadding
    sgl_ImGuiStyleVar_COUNT
} sgl_StyleVar;

typedef enum sgl_StyleCol
{
    sgl_ImGuiCol_Text,
    sgl_ImGuiCol_TextDisabled,
    sgl_ImGuiCol_WindowBg,              // Background of normal windows
    sgl_ImGuiCol_ChildBg,               // Background of child windows
    sgl_ImGuiCol_PopupBg,               // Background of popups, menus, tooltips windows
    sgl_ImGuiCol_Border,
    sgl_ImGuiCol_BorderShadow,
    sgl_ImGuiCol_FrameBg,               // Background of checkbox, radio button, plot, slider, text input
    sgl_ImGuiCol_FrameBgHovered,
    sgl_ImGuiCol_FrameBgActive,
    sgl_ImGuiCol_TitleBg,               // Title bar
    sgl_ImGuiCol_TitleBgActive,         // Title bar when focused
    sgl_ImGuiCol_TitleBgCollapsed,      // Title bar when collapsed
    sgl_ImGuiCol_MenuBarBg,
    sgl_ImGuiCol_ScrollbarBg,
    sgl_ImGuiCol_ScrollbarGrab,
    sgl_ImGuiCol_ScrollbarGrabHovered,
    sgl_ImGuiCol_ScrollbarGrabActive,
    sgl_ImGuiCol_CheckMark,             // Checkbox tick and RadioButton circle
    sgl_ImGuiCol_SliderGrab,
    sgl_ImGuiCol_SliderGrabActive,
    sgl_ImGuiCol_Button,
    sgl_ImGuiCol_ButtonHovered,
    sgl_ImGuiCol_ButtonActive,
    sgl_ImGuiCol_Header,                // Header* colors are used for CollapsingHeader, TreeNode, Selectable, MenuItem
    sgl_ImGuiCol_HeaderHovered,
    sgl_ImGuiCol_HeaderActive,
    sgl_ImGuiCol_Separator,
    sgl_ImGuiCol_SeparatorHovered,
    sgl_ImGuiCol_SeparatorActive,
    sgl_ImGuiCol_ResizeGrip,            // Resize grip in lower-right and lower-left corners of windows.
    sgl_ImGuiCol_ResizeGripHovered,
    sgl_ImGuiCol_ResizeGripActive,
    sgl_ImGuiCol_InputTextCursor,       // InputText cursor/caret
    sgl_ImGuiCol_TabHovered,            // Tab background, when hovered
    sgl_ImGuiCol_Tab,                   // Tab background, when tab-bar is focused & tab is unselected
    sgl_ImGuiCol_TabSelected,           // Tab background, when tab-bar is focused & tab is selected
    sgl_ImGuiCol_TabSelectedOverline,   // Tab horizontal overline, when tab-bar is focused & tab is selected
    sgl_ImGuiCol_TabDimmed,             // Tab background, when tab-bar is unfocused & tab is unselected
    sgl_ImGuiCol_TabDimmedSelected,     // Tab background, when tab-bar is unfocused & tab is selected
    sgl_ImGuiCol_TabDimmedSelectedOverline,//..horizontal overline, when tab-bar is unfocused & tab is selected
    sgl_ImGuiCol_PlotLines,
    sgl_ImGuiCol_PlotLinesHovered,
    sgl_ImGuiCol_PlotHistogram,
    sgl_ImGuiCol_PlotHistogramHovered,
    sgl_ImGuiCol_TableHeaderBg,         // Table header background
    sgl_ImGuiCol_TableBorderStrong,     // Table outer and header borders (prefer using Alpha=1.0 here)
    sgl_ImGuiCol_TableBorderLight,      // Table inner borders (prefer using Alpha=1.0 here)
    sgl_ImGuiCol_TableRowBg,            // Table row background (even rows)
    sgl_ImGuiCol_TableRowBgAlt,         // Table row background (odd rows)
    sgl_ImGuiCol_TextLink,              // Hyperlink color
    sgl_ImGuiCol_TextSelectedBg,        // Selected text inside an InputText
    sgl_ImGuiCol_TreeLines,             // Tree node hierarchy outlines when using ImGuiTreeNodeFlags_DrawLines
    sgl_ImGuiCol_DragDropTarget,        // Rectangle border highlighting a drop target
    sgl_ImGuiCol_DragDropTargetBg,      // Rectangle background highlighting a drop target
    sgl_ImGuiCol_UnsavedMarker,         // Unsaved Document marker (in window title and tabs)
    sgl_ImGuiCol_NavCursor,             // Color of keyboard/gamepad navigation cursor/rectangle, when visible
    sgl_ImGuiCol_NavWindowingHighlight, // Highlight window when using Ctrl+Tab
    sgl_ImGuiCol_NavWindowingDimBg,     // Darken/colorize entire screen behind the Ctrl+Tab window list, when active
    sgl_ImGuiCol_ModalWindowDimBg,      // Darken/colorize entire screen behind a modal window, when one is active
    sgl_ImGuiCol_COUNT,
} sgl_StyleCol;

SGL_API extern bool sgl_Begin(const char* title, bool* open, sgl_WindowFlags flags);
SGL_API extern void sgl_End();

SGL_API extern void sgl_Separator();
SGL_API extern void sgl_SeparatorText(const char* text);

SGL_API extern void sgl_BeginDisabled();
SGL_API extern void sgl_EndDisabled();

SGL_API extern void sgl_SameLine();
SGL_API extern void sgl_SameLineEx(float offsetFromStart, float spacing);

SGL_API extern void sgl_PushID(int id);
SGL_API extern void sgl_PopID();

SGL_API extern void sgl_SetStyleVar_float(sgl_StyleVar style, float val);
SGL_API extern void sgl_SetStyleVar_Vec2(sgl_StyleVar style, sgl_Vec2 val);
SGL_API extern void sgl_SetStyleVar_Colour(sgl_StyleCol style, sgl_Colour col);

SGL_API extern void sgl_PushStyleVar_float(sgl_StyleVar style, float val);
SGL_API extern void sgl_PushStyleVar_Vec2(sgl_StyleVar style, sgl_Vec2 val);
SGL_API extern void sgl_PopStyleVar();

SGL_API extern bool sgl_Button(const char* text);
SGL_API extern bool sgl_ButtonEx(const char* text, sgl_Vec2 size);

SGL_API extern void sgl_Text(const char* fmt, ...);
SGL_API extern void sgl_TextUnformatted(const char* text);
SGL_API extern void sgl_TextColoured(sgl_Colour colour, const char* fmt, ...);
SGL_API extern void sgl_TextUnformattedColoured(sgl_Colour colour, const char* text);

SGL_API extern bool sgl_BeginMainMenuBar();
SGL_API extern void sgl_EndMainMenuBar();

SGL_API extern bool sgl_BeginMenu(const char* text, bool enabled);
SGL_API extern void sgl_EndMenu();

SGL_API extern bool sgl_MenuItem(const char* text, const char* shortcut, bool selected, bool enabled);

SGL_API extern sgl_Vec2 sgl_GetContentRegionAvail();
SGL_API extern sgl_Vec2 sgl_CalcTextSize(const char* text, const char* textEnd, bool hideTextAfterDoubleHash, float wrapWidth);

SGL_END