using SGLNet.Graphics;
using SGLNet.Interop;
using System.Runtime.InteropServices;

namespace SGLNet
{
    public sealed class GraphicsDevice : IDisposable
    {
        private IntPtr mHandle;

        internal IntPtr Handle =>
            mHandle;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate IntPtr sgl_GraphicsDevice_Create_ptr(IntPtr window);
        private static sgl_GraphicsDevice_Create_ptr sgl_GraphicsDevice_Create;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_GraphicsDevice_Destroy_ptr(IntPtr device);
        private static sgl_GraphicsDevice_Destroy_ptr sgl_GraphicsDevice_Destroy;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_GraphicsDevice_SetClearColour_ptr(IntPtr device, Colour colour);
        private static sgl_GraphicsDevice_SetClearColour_ptr sgl_GraphicsDevice_SetClearColour;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_GraphicsDevice_Clear_ptr(IntPtr device, byte flag);
        private static sgl_GraphicsDevice_Clear_ptr sgl_GraphicsDevice_Clear;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_GraphicsDevice_Present_ptr(IntPtr device);
        private static sgl_GraphicsDevice_Present_ptr sgl_GraphicsDevice_Present;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_GraphicsDevice_Draw_ptr(IntPtr device, IntPtr va, IntPtr shr);

        public Colour ClearColour
        {
            set => sgl_GraphicsDevice_SetClearColour(mHandle, value);
        }

        public void Clear() =>
            sgl_GraphicsDevice_Clear(mHandle, 0b11);

        public void Present() =>
            sgl_GraphicsDevice_Present(mHandle);

        public void Draw(VertexArray va)
        {

        }

        public GraphicsDevice(Window window)
        {
            sgl_GraphicsDevice_Create ??= Native.GetFunction<sgl_GraphicsDevice_Create_ptr>(nameof(sgl_GraphicsDevice_Create));
            sgl_GraphicsDevice_Destroy ??= Native.GetFunction<sgl_GraphicsDevice_Destroy_ptr>(nameof(sgl_GraphicsDevice_Destroy));
            sgl_GraphicsDevice_SetClearColour ??= Native.GetFunction<sgl_GraphicsDevice_SetClearColour_ptr>(nameof(sgl_GraphicsDevice_SetClearColour));
            sgl_GraphicsDevice_Clear ??= Native.GetFunction<sgl_GraphicsDevice_Clear_ptr>(nameof(sgl_GraphicsDevice_Clear));
            sgl_GraphicsDevice_Present ??= Native.GetFunction<sgl_GraphicsDevice_Present_ptr>(nameof(sgl_GraphicsDevice_Present));

            mHandle = sgl_GraphicsDevice_Create(window.Handle);
        }

        public void Dispose()
        {
            sgl_GraphicsDevice_Destroy(mHandle);
            mHandle = IntPtr.Zero;

            GC.SuppressFinalize(this);
        }

        ~GraphicsDevice()
        {
            Dispose();
        }
    }
}