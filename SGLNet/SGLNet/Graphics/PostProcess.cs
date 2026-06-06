using SGLNet.Interop;
using System.Runtime.InteropServices;

namespace SGLNet.Graphics
{
    public sealed class PostProcess : IDisposable
    {
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate IntPtr sgl_PostProcess_Create_ptr(IntPtr device, IntPtr shader);
        private static sgl_PostProcess_Create_ptr sgl_PostProcess_Create;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_PostProcess_Destroy_ptr(IntPtr pp);
        private static sgl_PostProcess_Destroy_ptr sgl_PostProcess_Destroy;

        internal static void Init_FuncPtr()
        {
            sgl_PostProcess_Create ??= Native.GetFunction<sgl_PostProcess_Create_ptr>();
            sgl_PostProcess_Destroy ??= Native.GetFunction<sgl_PostProcess_Destroy_ptr>();
        }

        private IntPtr mHandle;
        internal IntPtr Handle =>
            mHandle;

        public void Dispose()
        {
            sgl_PostProcess_Destroy(mHandle);
            mHandle = IntPtr.Zero;

            GC.SuppressFinalize(this);
        }

        public PostProcess(GraphicsDevice device, Shader shader)
        {
            mHandle = sgl_PostProcess_Create(device.Handle, shader.Handle);
        }

        ~PostProcess() =>
            Dispose();
    }
}