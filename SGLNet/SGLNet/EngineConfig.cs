using System.Runtime.InteropServices;

namespace SGLNet
{
    [StructLayout(LayoutKind.Sequential)]
    public struct EngineConfig()
    {
        [MarshalAs(UnmanagedType.LPUTF8Str)]
        public string title = "SGLNet";

        public uint width = 1280;

        public uint height = 720;

        [MarshalAs(UnmanagedType.U1)]
        public bool resizable = true;
    }
}