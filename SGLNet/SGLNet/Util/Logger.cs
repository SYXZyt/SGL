using SGLNet.Interop;
using System.Runtime.InteropServices;

namespace SGLNet.Util
{
    public static class Logger
    {
        private static IntPtr StringToHGlobalUtf8(string text)
        {
            if (text is null)
                return IntPtr.Zero;

            byte[] utf8 = System.Text.Encoding.UTF8.GetBytes(text + '\0');
            IntPtr ptr = Marshal.AllocHGlobal(utf8.Length);
            Marshal.Copy(utf8, 0, ptr, utf8.Length);

            return ptr;
        }

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_Logger_Init_ptr();
        private static sgl_Logger_Init_ptr sgl_Logger_Init;
        public static void Init()
        {
            sgl_Logger_Init ??= Native.GetFunction<sgl_Logger_Init_ptr>(nameof(sgl_Logger_Init));
            sgl_Logger_Init();
        }

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_Logger_Shutdown_ptr();
        private static sgl_Logger_Shutdown_ptr sgl_Logger_Shutdown;
        public static void Shutdown()
        {
            sgl_Logger_Shutdown ??= Native.GetFunction<sgl_Logger_Shutdown_ptr>(nameof(sgl_Logger_Shutdown));
            sgl_Logger_Shutdown();
        }

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        private delegate bool sgl_Logger_IsLogging_ptr();
        private static sgl_Logger_IsLogging_ptr sgl_Logger_IsLogging;
        public static bool IsLogging
        {
            get
            {
                sgl_Logger_IsLogging ??= Native.GetFunction<sgl_Logger_IsLogging_ptr>(nameof(sgl_Logger_IsLogging));
                return sgl_Logger_IsLogging();
            }
        }

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_Log_ptr(IntPtr message);
        private static sgl_Log_ptr sgl_Log;
        private static sgl_Log_ptr sgl_LogWarning;
        private static sgl_Log_ptr sgl_LogError;
        private static sgl_Log_ptr sgl_LogSuccess;

        public static void Log(string message)
        {
            sgl_Log ??= Native.GetFunction<sgl_Log_ptr>(nameof(sgl_Log));
            
            IntPtr ptr = StringToHGlobalUtf8(message);
            try
            {
                sgl_Log(ptr);
            }
            finally
            {
                Marshal.FreeHGlobal(ptr);
            }
        }

        public static void LogWarning(string message)
        {
            sgl_LogWarning ??= Native.GetFunction<sgl_Log_ptr>(nameof(sgl_LogWarning));

            IntPtr ptr = StringToHGlobalUtf8(message);
            try
            {
                sgl_LogWarning(ptr);
            }
            finally
            {
                Marshal.FreeHGlobal(ptr);
            }
        }

        public static void LogError(string message)
        {
            sgl_LogError ??= Native.GetFunction<sgl_Log_ptr>(nameof(sgl_LogError));

            IntPtr ptr = StringToHGlobalUtf8(message);
            try
            {
                sgl_LogError(ptr);
            }
            finally
            {
                Marshal.FreeHGlobal(ptr);
            }
        }

        public static void LogSuccess(string message)
        {
            sgl_LogSuccess ??= Native.GetFunction<sgl_Log_ptr>(nameof(sgl_LogSuccess));

            IntPtr ptr = StringToHGlobalUtf8(message);
            try
            {
                sgl_LogSuccess(ptr);
            }
            finally
            {
                Marshal.FreeHGlobal(ptr);
            }
        }
    }
}