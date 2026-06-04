using SGLNet.Interop;
using System.Runtime.InteropServices;

namespace SGLNet.Graphics
{
    public sealed class Texture2D : Texture
    {
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private unsafe delegate IntPtr sgl_Texture2D_New_Source_ptr(IntPtr device, void* data, nuint dataSize);
        private static sgl_Texture2D_New_Source_ptr sgl_Texture2D_New_Source;

        private IntPtr mHandle;
        private readonly GraphicsDevice mGraphics;

        internal static new void Init_FuncPtr() =>
            sgl_Texture2D_New_Source ??= Native.GetFunction<sgl_Texture2D_New_Source_ptr>();

        internal override IntPtr Handle =>
            mHandle;

        public override void LoadFromMemory(byte[] bytes)
        {
            if (mHandle != IntPtr.Zero)
                throw new InvalidOperationException("Texture has already been created");

            unsafe
            {
                fixed (byte* ptr = bytes)
                    mHandle = sgl_Texture2D_New_Source(mGraphics.Handle, ptr, (nuint)bytes.Length);
            }
        }

        public override void LoadFromFile(string path)
        {
            if (mHandle != IntPtr.Zero)
                throw new InvalidOperationException("Texture has already been created");

            if (!File.Exists(path))
                throw new FileNotFoundException($"Could not find the file {path}");

            byte[] bytes = File.ReadAllBytes(path);
            LoadFromMemory(bytes);
        }

        protected override void Destruct() =>
            sgl_Texture_Destroy(mHandle);

        public override void Bind(TextureUnit unit) =>
            sgl_Texture_Bind(mHandle, unit);

        public Texture2D(GraphicsDevice device)
        {
            mHandle = IntPtr.Zero;
            mGraphics = device;
        }
    }
}