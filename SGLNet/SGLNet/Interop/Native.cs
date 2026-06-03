using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Text;

namespace SGLNet.Interop
{
    internal static class Native
    {
        private const string WindowsLib = "SGL";
        private const string LinuxLib = "libSGL";
        private const string DebugPostfix = "-d";

        private static string LibExtension =>
            OperatingSystem.IsWindows() ? ".dll" : ".so";

        private static string LibPath
        {
            get
            {
                StringBuilder sb = new();

                if (OperatingSystem.IsWindows())
                    sb.Append(WindowsLib);
                else
                    sb.Append(LinuxLib);

#if DEBUG
                sb.Append(DebugPostfix);
#endif

                sb.Append(LibExtension);
                return sb.ToString();
            }
        }

        private static readonly IntPtr mHandle;

        public static IntPtr LibHandle =>
            mHandle;

        public static FPtr GetFunction<FPtr>(string name) where FPtr : Delegate =>
            Marshal.GetDelegateForFunctionPointer<FPtr>(NativeLibrary.GetExport(LibHandle, name));

        public static FPtr GetFunction<FPtr>() where FPtr : Delegate
        {
            string name = typeof(FPtr).Name;
            const string suffix = "_ptr";

            if (name.EndsWith(suffix))
            {
                name = name[..^suffix.Length];
            }

            return GetFunction<FPtr>(name);
        }

        static Native()
        {
            if (!OperatingSystem.IsWindows() && !OperatingSystem.IsLinux())
                throw new PlatformNotSupportedException();

            string path = LibPath;

#if DEBUG
            Debug.WriteLine($"Loading native library: {path}");
#endif

            string fullPath = Path.Combine(AppContext.BaseDirectory, path);
            mHandle = NativeLibrary.Load(fullPath);
        }
    }
}