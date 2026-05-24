using System.Runtime.InteropServices;

namespace SGLNet.Interop
{
    internal static class Native
    {
        private const string WindowsLib = "SGL.dll";
        private const string LinuxLib = "libSGL.so";

        private static readonly IntPtr mHandle;

        public static IntPtr LibHandle =>
            mHandle;

        static Native()
        {
            string lib = OperatingSystem.IsWindows() ? WindowsLib : OperatingSystem.IsLinux() ? LinuxLib : throw new PlatformNotSupportedException();
            mHandle = NativeLibrary.Load(lib);
        }
    }
}