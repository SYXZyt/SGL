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
        private unsafe delegate void sgl_GraphicsDevice_Draw_ptr(IntPtr device, IntPtr va, IntPtr shr, IntPtr* buffers, nuint bufferCount);
        private static sgl_GraphicsDevice_Draw_ptr sgl_GraphicsDevice_Draw;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_GraphicsDevice_ImGui_Init_ptr(IntPtr device);
        private static sgl_GraphicsDevice_ImGui_Init_ptr sgl_GraphicsDevice_ImGui_Init;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_GraphicsDevice_ImGui_Shutdown_ptr(IntPtr device);
        private static sgl_GraphicsDevice_ImGui_Shutdown_ptr sgl_GraphicsDevice_ImGui_Shutdown;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_GraphicsDevice_ImGui_NewFrame_ptr(IntPtr device);
        private static sgl_GraphicsDevice_ImGui_NewFrame_ptr sgl_GraphicsDevice_ImGui_NewFrame;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_GraphicsDevice_ImGui_RenderDrawData_ptr(IntPtr device);
        private static sgl_GraphicsDevice_ImGui_RenderDrawData_ptr sgl_GraphicsDevice_ImGui_RenderDrawData;

        public Colour ClearColour
        {
            set => sgl_GraphicsDevice_SetClearColour(mHandle, value);
        }

        public void Clear() =>
            sgl_GraphicsDevice_Clear(mHandle, 0b11);

        public void Present() =>
            sgl_GraphicsDevice_Present(mHandle);

        public void Draw(VertexArray va, Shader shader, params IUniformBuffer[] uniformBuffers)
        {
            unsafe
            {
                int count = uniformBuffers?.Length ?? 0;

                IntPtr* bufferPtrs = stackalloc IntPtr[count];

                for (int i = 0; i < count; ++i)
                    bufferPtrs[i] = uniformBuffers[i].Handle;

                sgl_GraphicsDevice_Draw(mHandle, va.Handle, shader.Handle, bufferPtrs, (nuint)count);
            }
        }

        public void ImGui_Init() =>
            sgl_GraphicsDevice_ImGui_Init(mHandle);

        public void ImGui_Shutdown() =>
            sgl_GraphicsDevice_ImGui_Shutdown(mHandle);

        public void ImGui_NewFrame() =>
            sgl_GraphicsDevice_ImGui_NewFrame(mHandle);

        public void ImGui_RenderDrawData() =>
            sgl_GraphicsDevice_ImGui_RenderDrawData(mHandle);

        internal static void Init_FuncPtr()
        {
            sgl_GraphicsDevice_Create ??= Native.GetFunction<sgl_GraphicsDevice_Create_ptr>(nameof(sgl_GraphicsDevice_Create));
            sgl_GraphicsDevice_Destroy ??= Native.GetFunction<sgl_GraphicsDevice_Destroy_ptr>(nameof(sgl_GraphicsDevice_Destroy));
            sgl_GraphicsDevice_SetClearColour ??= Native.GetFunction<sgl_GraphicsDevice_SetClearColour_ptr>(nameof(sgl_GraphicsDevice_SetClearColour));
            sgl_GraphicsDevice_Clear ??= Native.GetFunction<sgl_GraphicsDevice_Clear_ptr>(nameof(sgl_GraphicsDevice_Clear));
            sgl_GraphicsDevice_Present ??= Native.GetFunction<sgl_GraphicsDevice_Present_ptr>(nameof(sgl_GraphicsDevice_Present));
            sgl_GraphicsDevice_Draw ??= Native.GetFunction<sgl_GraphicsDevice_Draw_ptr>(nameof(sgl_GraphicsDevice_Draw));

            sgl_GraphicsDevice_ImGui_Init ??= Native.GetFunction<sgl_GraphicsDevice_ImGui_Init_ptr>();
            sgl_GraphicsDevice_ImGui_Shutdown ??= Native.GetFunction<sgl_GraphicsDevice_ImGui_Shutdown_ptr>();
            sgl_GraphicsDevice_ImGui_NewFrame ??= Native.GetFunction<sgl_GraphicsDevice_ImGui_NewFrame_ptr>();
            sgl_GraphicsDevice_ImGui_RenderDrawData ??= Native.GetFunction<sgl_GraphicsDevice_ImGui_RenderDrawData_ptr>();
        }

        public GraphicsDevice(Window window)
        {
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