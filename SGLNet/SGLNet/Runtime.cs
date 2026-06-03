using SGLNet.Interop;
using System.Runtime.InteropServices;

namespace SGLNet
{
    public static class Runtime
    {
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_Runtime_Init_ptr();
        private static sgl_Runtime_Init_ptr sgl_Runtime_Init;
        public static void Init()
        {
            sgl_Runtime_Init ??= Native.GetFunction<sgl_Runtime_Init_ptr>(nameof(sgl_Runtime_Init));
            sgl_Runtime_Init();
        }

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_Runtime_Shutdown_ptr();
        private static sgl_Runtime_Shutdown_ptr sgl_Runtime_Shutdown;
        public static void Shutdown()
        {
            sgl_Runtime_Shutdown ??= Native.GetFunction<sgl_Runtime_Shutdown_ptr>(nameof(sgl_Runtime_Shutdown));
            sgl_Runtime_Shutdown();
        }
    }
}