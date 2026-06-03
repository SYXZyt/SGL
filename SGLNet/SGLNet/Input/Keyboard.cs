using SGLNet.Interop;
using System.Runtime.InteropServices;

namespace SGLNet.Input
{
    public static class Keyboard
    {
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate IntPtr sgl_Keyboard_New_ptr();
        private static sgl_Keyboard_New_ptr sgl_Keyboard_New;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_Keyboard_Destroy_ptr(IntPtr kb);
        private static sgl_Keyboard_Destroy_ptr sgl_Keyboard_Destroy;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_Keyboard_Update_ptr(IntPtr kb);
        private static sgl_Keyboard_Update_ptr sgl_Keyboard_Update;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        private delegate bool sgl_Keyboard_IsKeyDown_ptr(IntPtr kb, Key key);
        private static sgl_Keyboard_IsKeyDown_ptr sgl_Keyboard_IsKeyDown;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        private delegate bool sgl_Keyboard_IsKeyUp_ptr(IntPtr kb, Key key);
        private static sgl_Keyboard_IsKeyUp_ptr sgl_Keyboard_IsKeyUp;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        private delegate bool sgl_Keyboard_IsKeyPressed_ptr(IntPtr kb, Key key);
        private static sgl_Keyboard_IsKeyPressed_ptr sgl_Keyboard_IsKeyPressed;

        private static IntPtr sHandle;

        internal static IntPtr Handle =>
            sHandle;

        internal static void Init_FuncPtr()
        {
            sgl_Keyboard_New ??= Native.GetFunction<sgl_Keyboard_New_ptr>();
            sgl_Keyboard_Destroy ??= Native.GetFunction<sgl_Keyboard_Destroy_ptr>();

            sgl_Keyboard_Update ??= Native.GetFunction<sgl_Keyboard_Update_ptr>();

            sgl_Keyboard_IsKeyDown ??= Native.GetFunction<sgl_Keyboard_IsKeyDown_ptr>();
            sgl_Keyboard_IsKeyUp ??= Native.GetFunction<sgl_Keyboard_IsKeyUp_ptr>();
            sgl_Keyboard_IsKeyPressed ??= Native.GetFunction<sgl_Keyboard_IsKeyPressed_ptr>();
        }

        public static bool IsDown(Key key) =>
            sgl_Keyboard_IsKeyDown(sHandle, key);

        public static bool IsUp(Key key) =>
            sgl_Keyboard_IsKeyUp(sHandle, key);

        public static bool IsPressed(Key key) =>
            sgl_Keyboard_IsKeyPressed(sHandle, key);

        public static void Init() =>
            sHandle = sgl_Keyboard_New();

        public static void Shutdown() =>
            sgl_Keyboard_Destroy(sHandle);

        public static void Update() =>
            sgl_Keyboard_Update(sHandle);
    }
}