using System.Runtime.InteropServices;

namespace SGLNet
{
    public enum Backend : byte
    {
        OPENGL,
        DIRECTX11,
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct EngineConfig()
    {
        [MarshalAs(UnmanagedType.LPUTF8Str)]
        public string title = "SGLNet";

        public Backend backend = Backend.OPENGL;

        public uint width = 1280;

        public uint height = 720;

        [MarshalAs(UnmanagedType.U1)]
        public bool resizable = true;

        [MarshalAs(UnmanagedType.U1)]
        public bool enableImGui = false;
    }
}