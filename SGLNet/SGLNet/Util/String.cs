using SGLNet.Interop;
using System.Runtime.InteropServices;

namespace SGLNet.Util
{
    /// <summary>
    /// This string class requires explicit freeing. Only use where absolute necessary
    /// </summary>
    public sealed class String
    {
        [StructLayout(LayoutKind.Sequential)]
        public struct NativeString
        {
            public IntPtr str;
            public nuint len;
            public nuint capacity;
        }

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate NativeString sgl_MakeString_ptr([MarshalAs(UnmanagedType.LPUTF8Str)] string str);
        private static sgl_MakeString_ptr sgl_MakeString;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_FreeString_ptr(NativeString str);
        private static sgl_FreeString_ptr sgl_FreeString;

        private NativeString mNative;

        public NativeString Handle =>
            mNative;

        public void Free() =>
            sgl_FreeString(mNative);

        public String(string data)
        {
            sgl_MakeString ??= Native.GetFunction<sgl_MakeString_ptr>(nameof(sgl_MakeString));
            sgl_FreeString ??= Native.GetFunction<sgl_FreeString_ptr>(nameof(sgl_FreeString));
            
            mNative = sgl_MakeString(data);
        }
    }
}