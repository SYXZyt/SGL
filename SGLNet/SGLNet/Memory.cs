using SGLNet.Interop;
using System.Runtime.InteropServices;

namespace SGLNet
{
    public static class Memory
    {
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_Memory_StackTrace_ptr([MarshalAs(UnmanagedType.Bool)] bool enable);
        private static sgl_Memory_StackTrace_ptr sgl_Memory_StackTrace;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate nuint sgl_Memory_GetTotalAllocated_ptr();
        private static sgl_Memory_GetTotalAllocated_ptr sgl_Memory_GetTotalAllocated;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_Memory_AddTrack_ptr(IntPtr ptr, nuint size, [MarshalAs(UnmanagedType.LPUTF8Str)] string T);
        private static sgl_Memory_AddTrack_ptr sgl_Memory_AddTrack;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_Memory_PopTrack_ptr();
        private static sgl_Memory_PopTrack_ptr sgl_Memory_PopTrack;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate IntPtr sgl_Malloc_ptr(nuint size);
        private static sgl_Malloc_ptr sgl_Malloc;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_Free_ptr(IntPtr ptr);
        private static sgl_Free_ptr sgl_Free;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate IntPtr sgl_Realloc_ptr(IntPtr ptr, nuint newSize);
        private static sgl_Realloc_ptr sgl_Realloc;

        public static void StackTraceEnableIfDebug()
        {
#if DEBUG
            StackTrace(true);
#else
            StackTrace(false);
#endif
        }

        public static void StackTrace(bool enable) =>
            sgl_Memory_StackTrace(enable);

        public static ulong GetTotalAllocated() =>
            sgl_Memory_GetTotalAllocated();

        public static void AddTrack(IntPtr ptr, ulong size, string typename) =>
            sgl_Memory_AddTrack(ptr, (nuint)size, typename);

        public static void PopTrack() =>
            sgl_Memory_PopTrack();

        public static IntPtr Malloc(ulong size) =>
            sgl_Malloc((nuint)size);

        public static IntPtr Realloc(IntPtr ptr, ulong newSize) =>
            sgl_Realloc(ptr, (nuint)newSize);

        public static void Free(IntPtr ptr) =>
            sgl_Free(ptr);

        internal static void Init_FuncPtr()
        {
            sgl_Memory_StackTrace ??= Native.GetFunction<sgl_Memory_StackTrace_ptr>();
            sgl_Memory_GetTotalAllocated ??= Native.GetFunction<sgl_Memory_GetTotalAllocated_ptr>();
            sgl_Memory_AddTrack ??= Native.GetFunction<sgl_Memory_AddTrack_ptr>();
            sgl_Memory_PopTrack ??= Native.GetFunction<sgl_Memory_PopTrack_ptr>();
            sgl_Malloc ??= Native.GetFunction<sgl_Malloc_ptr>();
            sgl_Free ??= Native.GetFunction<sgl_Free_ptr>();
            sgl_Realloc ??= Native.GetFunction<sgl_Realloc_ptr>();
        }
    }
}