using SGLNet.Graphics;
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

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        internal unsafe delegate bool sgl_InputText_ptr(
            [MarshalAs(UnmanagedType.LPUTF8Str)] string text,
            byte* buffer,
            nuint bufferSize,
            uint flags
        );
        internal static sgl_InputText_ptr sgl_InputText;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        internal unsafe delegate bool sgl_InputInt_ptr(
            [MarshalAs(UnmanagedType.LPUTF8Str)] string text,
            int* v,
            int step,
            int stepFast,
            uint flags
        );
        internal static sgl_InputInt_ptr sgl_InputInt;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        internal unsafe delegate bool sgl_InputFloat_ptr(
            [MarshalAs(UnmanagedType.LPUTF8Str)] string text,
            float* v,
            float step,
            float stepFast,
            uint flags
        );
        internal static sgl_InputFloat_ptr sgl_InputFloat;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        internal unsafe delegate bool sgl_InputVec2_ptr(
            [MarshalAs(UnmanagedType.LPUTF8Str)] string text,
            Vec2* v,
            uint flags
        );
        internal static sgl_InputVec2_ptr sgl_InputVec2;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        internal unsafe delegate bool sgl_InputMat4_ptr(
            [MarshalAs(UnmanagedType.LPUTF8Str)] string text,
            Mat4* v,
            uint flags
            );
        internal static sgl_InputMat4_ptr sgl_InputMat4;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        internal unsafe delegate bool sgl_SliderFloat_ptr(
            [MarshalAs(UnmanagedType.LPUTF8Str)] string text,
            float* v,
            float min,
            float max
        );
        internal static sgl_SliderFloat_ptr sgl_SliderFloat;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        internal unsafe delegate bool sgl_SliderInt_ptr(
            [MarshalAs(UnmanagedType.LPUTF8Str)] string text,
            int* v,
            int min,
            int max
        );
        internal static sgl_SliderInt_ptr sgl_SliderInt;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        internal unsafe delegate bool sgl_DragFloat_ptr(
            [MarshalAs(UnmanagedType.LPUTF8Str)] string text,
            float* v,
            float speed,
            float min,
            float max
        );
        internal static sgl_DragFloat_ptr sgl_DragFloat;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        internal unsafe delegate bool sgl_DragInt_ptr(
            [MarshalAs(UnmanagedType.LPUTF8Str)] string text,
            int* v,
            float speed,
            int min,
            int max
        );
        internal static sgl_DragInt_ptr sgl_DragInt;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        internal unsafe delegate bool sgl_DragVec2_ptr(
            [MarshalAs(UnmanagedType.LPUTF8Str)] string text,
            Vec2* v,
            float speed,
            float min,
            float max
        );
        internal static sgl_DragVec2_ptr sgl_DragVec2;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        internal unsafe delegate bool sgl_DragMat4_ptr(
            [MarshalAs(UnmanagedType.LPUTF8Str)] string text,
            Mat4* v,
            float speed,
            float min,
            float max
        );
        internal static sgl_DragMat4_ptr sgl_DragMat4;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        internal unsafe delegate bool sgl_Checkbox_ptr(
            [MarshalAs(UnmanagedType.LPUTF8Str)] string text,
            bool* v
        );
        internal static sgl_Checkbox_ptr sgl_Checkbox;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        internal unsafe delegate bool sgl_CollapsableHeader_ptr(
            [MarshalAs(UnmanagedType.LPUTF8Str)] string text
        );
        internal static sgl_CollapsableHeader_ptr sgl_CollapsableHeader;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        internal unsafe delegate bool sgl_TreeNode_ptr(
            [MarshalAs(UnmanagedType.LPUTF8Str)] string text
        );
        internal static sgl_TreeNode_ptr sgl_TreeNode;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void sgl_TreePop_ptr();
        internal static sgl_TreePop_ptr sgl_TreePop;

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

            sgl_InputText ??= Native.GetFunction<sgl_InputText_ptr>();
            sgl_InputInt ??= Native.GetFunction<sgl_InputInt_ptr>();
            sgl_InputFloat ??= Native.GetFunction<sgl_InputFloat_ptr>();
            sgl_InputVec2 ??= Native.GetFunction<sgl_InputVec2_ptr>();
            sgl_InputMat4 ??= Native.GetFunction<sgl_InputMat4_ptr>();
            sgl_SliderFloat ??= Native.GetFunction<sgl_SliderFloat_ptr>();
            sgl_SliderInt ??= Native.GetFunction<sgl_SliderInt_ptr>();
            sgl_DragFloat ??= Native.GetFunction<sgl_DragFloat_ptr>();
            sgl_DragInt ??= Native.GetFunction<sgl_DragInt_ptr>();
            sgl_DragVec2 ??= Native.GetFunction<sgl_DragVec2_ptr>();
            sgl_DragMat4 ??= Native.GetFunction<sgl_DragMat4_ptr>();
            sgl_Checkbox ??= Native.GetFunction<sgl_Checkbox_ptr>();
            sgl_CollapsableHeader ??= Native.GetFunction<sgl_CollapsableHeader_ptr>();
            sgl_TreeNode ??= Native.GetFunction<sgl_TreeNode_ptr>();
            sgl_TreePop ??= Native.GetFunction<sgl_TreePop_ptr>();
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

        public enum InputTextFlags
        {
            NONE = 0,
            CHARS_DECIMAL = 1 << 0,
            CHARS_HEXADECIMCAL = 1 << 1,
            CHARS_SCIENTIFIC = 1 << 2,
            CHARS_UPPERCASE = 1 << 3,
            CHARS_NO_BLANK = 1 << 4,

            ALLOW_TAB_INPUT = 1 << 5,
            ENTER_RETURNS_TRUE = 1 << 6,
            ESCAPE_CLEARS_ALL = 1 << 7,
            CTRL_ENTER_FOR_NEWLINE = 1 << 8,

            READONLY = 1 << 9,
            PASSWORD = 1 << 10,
            ALWAYS_OVERWRITE = 1 << 11,
            AUTO_SELECT_ALL = 1 << 12,
            PARSE_EMPTY_REF_VAL = 1 << 13,
            DISPLAY_EMPTY_REF_VAL = 1 << 14,
            NO_HORIZONTAL_SCROLL = 1 << 15,
            NO_UNDO_REDO = 1 << 16,

            ELIDE_LEFT = 1 << 17,

            // Currently we don't support callbacks
            CALLBACK_COMPLETION = 1 << 18,
            CALLBACK_HISTORY = 1 << 19,
            CALLBACK_ALWAYS = 1 << 20,
            CALLBACK_CHAR_FILTER = 1 << 21,
            CALLBACK_RESIZE = 1 << 22,
            CALLBACK_EDIT = 1 << 23,

            WORD_WRAP = 1 << 24,
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

        public static bool InputText(string label, ref string value, int bufferSize = 256, InputTextFlags flags = InputTextFlags.NONE)
        {
            unsafe
            {
                byte* buffer = stackalloc byte[bufferSize];

                if (!string.IsNullOrEmpty(value))
                {
                    byte[] utf8 = System.Text.Encoding.UTF8.GetBytes(value);
                    int len = Mathf.Min(utf8.Length, bufferSize - 1);

                    for (int i = 0; i < len; ++i)
                        buffer[i] = utf8[i];

                    buffer[len] = 0;
                }
                else
                    buffer[0] = 0;

                bool changed = sgl_InputText(label, buffer, (UIntPtr)buffer, (uint)flags);

                if (changed)
                {
                    int len = 0;
                    while (len < bufferSize && buffer[len] != 0)
                        ++len;

                    value = System.Text.Encoding.UTF8.GetString(buffer, len);
                }

                return changed;
            }
        }

        public static bool InputInt(string label, ref int value, int step = 1, int stepFast = 100, InputTextFlags flags = InputTextFlags.NONE)
        {
            unsafe
            {
                fixed (int* p = &value)
                    return sgl_InputInt(label, p, step, stepFast, (uint)flags);
            }
        }

        public static bool InputFloat(string label, ref float value, float step = 0, float stepFast = 0, InputTextFlags flags = InputTextFlags.NONE)
        {
            unsafe
            {
                fixed (float* p = &value)
                    return sgl_InputFloat(label, p, step, stepFast, (uint)flags);
            }
        }

        public static bool InputVec2(string label, ref Vec2 value, InputTextFlags flags = InputTextFlags.NONE)
        {
            unsafe
            {
                fixed (Vec2* p = &value)
                    return sgl_InputVec2(label, p, (uint)flags);
            }
        }

        public static bool InputMat4(string label, ref Mat4 value, InputTextFlags flags = InputTextFlags.NONE)
        {
            unsafe
            {
                fixed (Mat4* p = &value)
                    return sgl_InputMat4(label, p, (uint)flags);
            }
        }

        public static bool SliderFloat(string label, ref float value, float min, float max)
        {
            unsafe
            {
                fixed (float* p = &value)
                    return sgl_SliderFloat(label, p, min, max);
            }
        }

        public static bool SliderInt(string label, ref int value, int min, int max)
        {
            unsafe
            {
                fixed (int* p = &value)
                    return sgl_SliderInt(label, p, min, max);
            }
        }

        public static bool DragFloat(string label, ref float value, float speed, float min = 0, float max = 0)
        {
            unsafe
            {
                fixed (float* p = &value)
                    return sgl_DragFloat(label, p, speed, min, max);
            }
        }

        public static bool DragInt(string label, ref int value, float speed, int min = 0, int max = 0)
        {
            unsafe
            {
                fixed (int* p = &value)
                    return sgl_DragInt(label, p, speed, min, max);
            }
        }

        public static bool DragVec2(string label, ref Vec2 value, float speed, float min = 0, float max = 0)
        {
            unsafe
            {
                fixed (Vec2* p = &value)
                    return sgl_DragVec2(label, p, speed, min, max);
            }
        }

        public static bool DragMat4(string label, ref Mat4 value, float speed, float min = 0, float max = 0)
        {
            unsafe
            {
                fixed (Mat4* p = &value)
                    return sgl_DragMat4(label, p, speed, min, max);
            }
        }

        public static bool Checkbox(string label, ref bool v)
        {
            unsafe
            {
                fixed (bool* p = &v)
                    return sgl_Checkbox(label, p);
            }
        }

        public static bool CollapsableHeader(string label) =>
            sgl_CollapsableHeader(label);

        public static bool TreeNode(string label) =>
            sgl_TreeNode(label);

        public static void TreePop() =>
            sgl_TreePop();
    }

    [AttributeUsage(AttributeTargets.Field)]
    public sealed class ImGuiInputAttribute(bool hasStep = false, float step = 0f, bool hasStepFast = false, float stepFast = 0f) : Attribute
    {
        public bool HasStep = hasStep;
        public float Step = step;

        public bool HasStepFast = hasStepFast;
        public float StepFast = stepFast;
    }

    [AttributeUsage(AttributeTargets.Field)]
    public sealed class ImGuiDragAttribute(float speed = 0.1f, float min = 0f, float max = 0f) : Attribute
    {
        public float Speed = speed;
        public float Min = min;
        public float Max = max;
    }

    public static class ImGuiInspector
    {
        public static bool Edit<T>(UniformBuffer<T> uniformBuffer) where T : unmanaged
        {
            if (Edit(ref uniformBuffer.Data))
            {
                uniformBuffer.Upload();
                return true;
            }

            return false;
        }

        public static bool Edit<T>(ref T value) where T : unmanaged
        {
            bool changed = false;

            object boxed = value;

            foreach (var field in typeof(T).GetFields())
            {
                object fieldValue = field.GetValue(boxed);

                var attrDrag = (ImGuiDragAttribute)Attribute.GetCustomAttribute(field, typeof(ImGuiDragAttribute));
                var attrInput = (ImGuiInputAttribute)Attribute.GetCustomAttribute(field, typeof(ImGuiInputAttribute));

                switch (fieldValue)
                {
                    case int i:
                    {
                        int v = i;

                        if (attrDrag is not null)
                        {
                            float speed = attrDrag.Speed;
                            int min = (int)attrDrag.Min;
                            int max = (int)attrDrag.Max;

                            if (ImGui.DragInt(field.Name, ref v, speed, min, max))
                            {
                                field.SetValue(boxed, v);
                                changed = true;
                            }
                        }
                        else if (attrInput is not null)
                        {
                            bool hasStep = attrInput.HasStep;
                            bool hasStepFast = attrInput.HasStepFast;

                            int step = attrInput.HasStep ? (int)attrInput.Step : 1;
                            int stepFast = attrInput.HasStepFast ? (int)attrInput.StepFast : 100;

                            if (ImGui.InputInt(field.Name, ref v, step, stepFast))
                            {
                                field.SetValue(boxed, v);
                                changed = true;
                            }
                        }
                        else
                        {
                            if (ImGui.InputInt(field.Name, ref v))
                            {
                                field.SetValue(boxed, v);
                                changed = true;
                            }
                        }

                        break;
                    }

                    case float f:
                    {
                        float v = f;

                        if (attrDrag is not null)
                        {
                            float speed = attrDrag.Speed;
                            float min = attrDrag.Min;
                            float max = attrDrag.Max;

                            if (ImGui.DragFloat(field.Name, ref v, speed, min, max))
                            {
                                field.SetValue(boxed, v);
                                changed = true;
                            }
                        }
                        else if (attrInput is not null)
                        {
                            bool hasStep = attrInput.HasStep;
                            bool hasStepFast = attrInput.HasStepFast;

                            float step = attrInput.HasStep ? attrInput.Step : 0f;
                            float stepFast = attrInput.HasStepFast ? attrInput.StepFast : 0f;

                            if (ImGui.InputFloat(field.Name, ref v, step, stepFast))
                            {
                                field.SetValue(boxed, v);
                                changed = true;
                            }
                        }
                        else
                        {
                            if (ImGui.InputFloat(field.Name, ref v))
                            {
                                field.SetValue(boxed, v);
                                changed = true;
                            }
                        }

                        break;
                    }

                    case bool b:
                    {
                        if (ImGui.Checkbox(field.Name, ref b))
                        {
                            field.SetValue(boxed, b);
                            changed = true;
                        }

                        break;
                    }

                    case Vec2 v:
                    {
                        Vec2 v2 = v;

                        if (attrDrag is not null)
                        {
                            float speed = attrDrag.Speed;
                            float min = attrDrag.Min;
                            float max = attrDrag.Max;

                            if (ImGui.DragVec2(field.Name, ref v2, speed, min, max))
                            {
                                field.SetValue(boxed, v2);
                                changed = true;
                            }
                        }
                        else
                        {
                            if (ImGui.InputVec2(field.Name, ref v2))
                            {
                                field.SetValue(boxed, v2);
                                changed = true;
                            }
                        }

                        break;
                    }

                    case Mat4 m:
                    {
                        Mat4 m4 = m;

                        if (attrDrag is not null)
                        {
                            float speed = attrDrag.Speed;
                            float min = attrDrag.Min;
                            float max = attrDrag.Max;

                            if (ImGui.DragMat4(field.Name, ref m4, speed, min, max))
                            {
                                field.SetValue(boxed, m4);
                                changed = true;
                            }
                        }
                        else
                        {
                            if (ImGui.InputMat4(field.Name, ref m4))
                            {
                                field.SetValue(boxed, m4);
                                changed = true;
                            }
                        }

                        break;
                    }
                }
            }

            if (changed)
                value = (T)boxed;

            return changed;
        }
    }
}