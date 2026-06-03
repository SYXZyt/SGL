using SGLNet.Interop;
using SGLNet.Maths;
using System.Runtime.InteropServices;

using static SGLNet.ImGuiNative;

namespace SGLNet
{
    internal static class ImGuiNative
    {
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        internal unsafe delegate bool sgl_Begin_ptr([MarshalAs(UnmanagedType.LPUTF8Str)] string title, bool* open, uint flags);
        internal static sgl_Begin_ptr sgl_Begin;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void sgl_End_ptr();
        internal static sgl_End_ptr sgl_End;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void sgl_Separator_ptr();
        internal static sgl_Separator_ptr sgl_Separator;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void sgl_SeparatorText_ptr([MarshalAs(UnmanagedType.LPUTF8Str)] string label);
        internal static sgl_SeparatorText_ptr sgl_SeparatorText;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void sgl_BeginDisabled_ptr();
        internal static sgl_BeginDisabled_ptr sgl_BeginDisabled;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void sgl_EndDisabled_ptr();
        internal static sgl_EndDisabled_ptr sgl_EndDisabled;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void sgl_SameLine_ptr();
        internal static sgl_SameLine_ptr sgl_SameLine;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void sgl_SameLineEx_ptr(float offset, float spacing);
        internal static sgl_SameLineEx_ptr sgl_SameLineEx;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void sgl_PushID_ptr(int id);
        internal static sgl_PushID_ptr sgl_PushID;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void sgl_PopID_ptr();
        internal static sgl_PopID_ptr sgl_PopID;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void sgl_SetStyleVar_float_ptr(uint style, float val);
        internal static sgl_SetStyleVar_float_ptr sgl_SetStyleVar_float;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void sgl_SetStyleVar_Vec2_ptr(uint style, Vec2 val);
        internal static sgl_SetStyleVar_Vec2_ptr sgl_SetStyleVar_Vec2;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void sgl_SetStyleVar_Colour_ptr(uint style, Colour col);
        internal static sgl_SetStyleVar_Colour_ptr sgl_SetStyleVar_Colour;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void sgl_PushStyleVar_float_ptr(uint style, float val);
        internal static sgl_PushStyleVar_float_ptr sgl_PushStyleVar_float;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void sgl_PushStyleVar_Vec2_ptr(uint style, Vec2 val);
        internal static sgl_PushStyleVar_Vec2_ptr sgl_PushStyleVar_Vec2;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void sgl_PopStyleVar_ptr();
        internal static sgl_PopStyleVar_ptr sgl_PopStyleVar;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        internal delegate bool sgl_Button_ptr([MarshalAs(UnmanagedType.LPUTF8Str)] string text);
        internal static sgl_Button_ptr sgl_Button;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        internal delegate bool sgl_ButtonEx_ptr([MarshalAs(UnmanagedType.LPUTF8Str)] string text, Vec2 size);
        internal static sgl_ButtonEx_ptr sgl_ButtonEx;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void sgl_TextUnformatted_ptr([MarshalAs(UnmanagedType.LPUTF8Str)] string text);
        internal static sgl_TextUnformatted_ptr sgl_TextUnformatted;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void sgl_TextUnformattedColoured_ptr(Colour colour, [MarshalAs(UnmanagedType.LPUTF8Str)] string text);
        internal static sgl_TextUnformattedColoured_ptr sgl_TextUnformattedColoured;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        internal delegate bool sgl_BeginMainMenuBar_ptr();
        internal static sgl_BeginMainMenuBar_ptr sgl_BeginMainMenuBar;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void sgl_EndMainMenuBar_ptr();
        internal static sgl_EndMainMenuBar_ptr sgl_EndMainMenuBar;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        internal delegate bool sgl_BeginMenu_ptr([MarshalAs(UnmanagedType.LPUTF8Str)] string text, [MarshalAs(UnmanagedType.U1)] bool enabled);
        internal static sgl_BeginMenu_ptr sgl_BeginMenu;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void sgl_EndMenu_ptr();
        internal static sgl_EndMenu_ptr sgl_EndMenu;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        internal delegate bool sgl_MenuItem_ptr(
            [MarshalAs(UnmanagedType.LPUTF8Str)] string text,
            [MarshalAs(UnmanagedType.LPUTF8Str)] string shortcut,
            [MarshalAs(UnmanagedType.U1)] bool selected,
            [MarshalAs(UnmanagedType.U1)] bool enabled
        );
        internal static sgl_MenuItem_ptr sgl_MenuItem;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate Vec2 sgl_GetContentRegionAvail_ptr();
        internal static sgl_GetContentRegionAvail_ptr sgl_GetContentRegionAvail;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate Vec2 sgl_CalcTextSize_ptr(
            [MarshalAs(UnmanagedType.LPUTF8Str)] string text,
            [MarshalAs(UnmanagedType.LPUTF8Str)] string textEnd,
            [MarshalAs(UnmanagedType.U1)] bool hideTextAfterDoubleHash,
            float wrapWidth
        );
        internal static sgl_CalcTextSize_ptr sgl_CalcTextSize;

        internal static void Init_FuncPtr()
        {
            sgl_Begin ??= Native.GetFunction<sgl_Begin_ptr>();
            sgl_End ??= Native.GetFunction<sgl_End_ptr>();

            sgl_Separator ??= Native.GetFunction<sgl_Separator_ptr>();
            sgl_SeparatorText ??= Native.GetFunction<sgl_SeparatorText_ptr>();

            sgl_BeginDisabled ??= Native.GetFunction<sgl_BeginDisabled_ptr>();
            sgl_EndDisabled ??= Native.GetFunction<sgl_EndDisabled_ptr>();

            sgl_SameLine ??= Native.GetFunction<sgl_SameLine_ptr>();
            sgl_SameLineEx ??= Native.GetFunction<sgl_SameLineEx_ptr>();

            sgl_PushID ??= Native.GetFunction<sgl_PushID_ptr>();
            sgl_PopID ??= Native.GetFunction<sgl_PopID_ptr>();

            sgl_SetStyleVar_float ??= Native.GetFunction<sgl_SetStyleVar_float_ptr>();
            sgl_SetStyleVar_Vec2 ??= Native.GetFunction<sgl_SetStyleVar_Vec2_ptr>();
            sgl_SetStyleVar_Colour ??= Native.GetFunction<sgl_SetStyleVar_Colour_ptr>();

            sgl_PushStyleVar_float ??= Native.GetFunction<sgl_PushStyleVar_float_ptr>();
            sgl_PushStyleVar_Vec2 ??= Native.GetFunction<sgl_PushStyleVar_Vec2_ptr>();
            sgl_PopStyleVar ??= Native.GetFunction<sgl_PopStyleVar_ptr>();

            sgl_Button ??= Native.GetFunction<sgl_Button_ptr>();
            sgl_ButtonEx ??= Native.GetFunction<sgl_ButtonEx_ptr>();

            sgl_TextUnformatted ??= Native.GetFunction<sgl_TextUnformatted_ptr>();
            sgl_TextUnformattedColoured ??= Native.GetFunction<sgl_TextUnformattedColoured_ptr>();

            sgl_BeginMainMenuBar ??= Native.GetFunction<sgl_BeginMainMenuBar_ptr>();
            sgl_EndMainMenuBar ??= Native.GetFunction<sgl_EndMainMenuBar_ptr>();

            sgl_BeginMenu ??= Native.GetFunction<sgl_BeginMenu_ptr>();
            sgl_EndMenu ??= Native.GetFunction<sgl_EndMenu_ptr>();

            sgl_MenuItem ??= Native.GetFunction<sgl_MenuItem_ptr>();

            sgl_GetContentRegionAvail ??= Native.GetFunction<sgl_GetContentRegionAvail_ptr>();
            sgl_CalcTextSize ??= Native.GetFunction<sgl_CalcTextSize_ptr>();
        }
    }

    public static class ImGui
    {
        public enum WindowFlags : uint
        {
            NONE = 0,
            NO_TITLE_BAR = 1 << 0,
            NO_RESIZE = 1 << 1,
            NO_MOVE = 1 << 2,
            NO_SCROLLBAR = 1 << 3,
            NO_SCROLLBAR_WITH_MOUSE = 1 << 4,
            NO_COLLAPSE = 1 << 5,
            ALWAYS_AUTO_RESIZE = 1 << 6,
            NO_BACKGROUND = 1 << 7,
            NO_SAVED_SETTINGS = 1 << 8,
            NO_MOUSE_INPUTS = 1 << 9,
            MENUBAR = 1 << 10,
            HORIZONTAL_SCROLLBAR = 1 << 11,
            NO_FOCUS_ON_APPEARING = 1 << 12,
            NO_BRING_TO_FRONT_ON_FOCUS = 1 << 13,
            ALWAYS_VERTICAL_SCROLLBAR = 1 << 14,
            ALWAYS_HORIZONTAL_SCROLLBAR = 1 << 15,
            NO_NAV_INPUTS = 1 << 16,
            NO_NAV_FOCUS = 1 << 17,
            UNSAVED_DOCUMENT = 1 << 18,

            NO_NAV = NO_NAV_INPUTS | NO_NAV_FOCUS,
            NO_DECORATION = NO_TITLE_BAR | NO_RESIZE | NO_SCROLLBAR | NO_COLLAPSE,
            NO_INPUTS = NO_MOUSE_INPUTS | NO_NAV_INPUTS | NO_NAV_FOCUS,
        }

        public enum StyleVar : uint
        {
            ALPHA,
            DISABLED_ALPHA,
            WINDOW_PADDING,
            WINDOW_ROUNDING,
            WINDOW_BORDER_SIZE,
            WINDOW_MIN_SIZE,
            WINDOW_TITLE_ALIGN,
            CHILD_ROUNDING,
            CHILD_BORDER_SIZE,
            POPUP_ROUNDING,
            POPUP_BORDER_SIZE,
            FRAME_PADDING,
            FRAME_ROUNDING,
            FRAME_BORDER_SIZE,
            ITEM_SPACING,
            ITEM_INNER_SPACING,
            INDENT_SPACING,
            CELL_PADDING,
            SCROLLBAR_SIZE,
            SCROLLBAR_ROUNDING,
            SCROLLBAR_PADDING,
            GRAB_MIN_SIZE,
            GRAB_ROUNDING,
            IMAGE_ROUNDING,
            IMAGE_BORDER_SIZE,
            TAB_ROUNDING,
            TAB_BORDER_SIZE,
            TAB_MIN_WIDTH_BASE,
            TAB_MIN_WIDTH_SHRINK,
            TAB_BAR_BORDER_SIZE,
            TAB_BAR_OVERLINE_SIZE,
            TABLE_ANGLED_HEADERS_ANGLE,
            TABLE_ANGLED_HEADERS_TEXT_ALIGN,
            TREE_LINES_SIZE,
            TREE_LINES_ROUNDING,
            BUTTON_TEXT_ALIGN,
            SELECTABLE_TEXT_ALIGN,
            SEPARATOR_SIZE,
            SEPARATOR_TEXT_BORDER_SIZE,
            SEPARATOR_TEXT_ALIGN,
            SEPARATOR_TEXT_PADDING
        }

        public enum StyleCol : uint
        {
            TEXT,
            TEXT_DISABLED,
            WINDOW_BG,
            CHILD_BG,
            POPUP_BG,
            BORDER,
            BORDER_SHADOW,
            FRAME_BG,
            FRAME_BG_HOVERED,
            FRAME_BG_ACTIVE,
            TITLE_BG,
            TITLE_BG_ACTIVE,
            TITLE_BG_COLLAPSED,
            MENUBAR_BG,
            SCROLLBAR_BG,
            SCROLLBAR_GRAB,
            SCROLLBAR_GRAB_HOVERED,
            SCROLLBAR_GRAB_ACTIVE,
            CHECK_MARK,
            SLIDER_GRAB,
            SLIDER_GRAB_ACTIVE,
            BUTTON,
            BUTTON_HOVERED,
            BUTTON_ACTIVE,
            HEADER,
            HEADER_HOVERED,
            HEADER_ACTIVE,
            SEPARATOR,
            SEPARATOR_HOVERED,
            SEPARATOR_ACTIVE,
            RESIZE_GRIP,
            RESIZE_GRIP_HOVERED,
            RESIZE_GRIP_ACTIVE,
            INPUT_TEXT_CURSOR,
            TAB_HOVERED,
            TAB,
            TAB_SELECTED,
            TAB_SELECTED_OVERLINE,
            TAB_DIMMED,
            TAB_DIMMED_SELECTED,
            TAB_DIMMED_SELECTED_OVERLINE,
            PLOT_LINES,
            PLOT_LINES_HOVERED,
            PLOT_HISTOGRAM,
            PLOT_HISTOGRAM_HOVERED,
            TABLE_HEADER_BG,
            TABLE_BORDER_STRONG,
            TABLE_BORDER_LIGHT,
            TABLE_ROW_BG,
            TABLE_ROW_BG_ALT,
            TEXT_LINK,
            TEXT_SELECTED_BG,
            TREE_LINES,
            DRAG_DROP_TARGET,
            DRAG_DROP_TARGET_BG,
            UNSAVED_MARKER,
            NAV_CURSOR,
            NAV_WINDOWING_HIGHLIGHT,
            NAV_WINDOWING_DIM_BG,
            MODAL_WINDOW_DIM_BG,
        }

        public static bool Begin(string title, ref bool open, WindowFlags flags = WindowFlags.NONE)
        {
            unsafe
            {
                fixed (bool* b = &open)
                    return sgl_Begin(title, b, (uint)flags);
            }
        }

        public static bool Begin(string title, WindowFlags flags = WindowFlags.NONE)
        {
            unsafe
            {
                return sgl_Begin(title, null, (uint)flags);
            }
        }

        public static void End() =>
            sgl_End();

        public static void Separator() =>
            sgl_Separator();

        public static void Separator(string label) =>
            sgl_SeparatorText(label);

        public static void BeginDisabled() =>
            sgl_BeginDisabled();

        public static void EndDisabled() =>
            sgl_EndDisabled();

        public static void SameLine() =>
            sgl_SameLine();

        public static void SameLine(float offset = 0, float spacing = -1) =>
            sgl_SameLineEx(offset, spacing);

        public static void PushID(int id) =>
            sgl_PushID(id);

        public static void PopID() =>
            sgl_PopID();

        public static void SetStyle(StyleVar style, float val) =>
            sgl_SetStyleVar_float((uint)style, val);

        public static void SetStyle(StyleVar style, in Vec2 val) =>
            sgl_SetStyleVar_Vec2((uint)style, val);

        public static void SetStyle(StyleCol style, in Colour val) =>
            sgl_SetStyleVar_Colour((uint)style, val);

        public static void PushStyle(StyleVar style, float val) =>
            sgl_PushStyleVar_float((uint)style, val);

        public static void PushStyle(StyleVar style, in Vec2 val) =>
            sgl_PushStyleVar_Vec2((uint)style, val);

        public static void PopStyle() =>
            sgl_PopStyleVar();

        public static bool Button(string label) =>
            sgl_Button(label);

        public static bool Button(string label, in Vec2 size) =>
            sgl_ButtonEx(label, size);

        public static void Text(string label) =>
            sgl_TextUnformatted(label);

        public static void TextUnformatted(string label) =>
            sgl_TextUnformatted(label);

        public static void TextColoured(string label, in Colour colour) =>
            sgl_TextUnformattedColoured(colour, label);

        public static void TextUnformattedColoured(string label, in Colour colour) =>
            sgl_TextUnformattedColoured(colour, label);

        public static bool BeginMainMenuBar() =>
            sgl_BeginMainMenuBar();

        public static void EndMainMenuBar() =>
            sgl_EndMainMenuBar();

        public static bool BeginMenu(string label, bool enabled) =>
            sgl_BeginMenu(label, enabled);

        public static void EndMenu() =>
            sgl_EndMenu();

        public static bool MenuItem(string text, string shortcut = "", bool selected = false, bool enabled = true) =>
            sgl_MenuItem(text, shortcut, selected, enabled);

        public static Vec2 GetContentRegionAvail() =>
            sgl_GetContentRegionAvail();

        public static Vec2 CalcTextSize(string text, string textEnd = "", bool hideTextAfterDoubleHash = false, float wrapWidth = -1) =>
            sgl_CalcTextSize(text, textEnd, hideTextAfterDoubleHash, wrapWidth);
    }
}