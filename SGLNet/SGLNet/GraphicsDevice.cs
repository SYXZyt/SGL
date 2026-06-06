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

        private VertexLayout mPostProcessLayout;

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
        private delegate void sgl_GraphicsDevice_BeginFrame_ptr(IntPtr device);
        private static sgl_GraphicsDevice_BeginFrame_ptr sgl_GraphicsDevice_BeginFrame;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_GraphicsDevice_EndFrame_ptr(IntPtr device);
        private static sgl_GraphicsDevice_EndFrame_ptr sgl_GraphicsDevice_EndFrame;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_GraphicsDevice_SwapBuffer_ptr(IntPtr device);
        private static sgl_GraphicsDevice_SwapBuffer_ptr sgl_GraphicsDevice_SwapBuffer;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private unsafe delegate void sgl_GraphicsDevice_Draw_ptr(IntPtr device, IntPtr va, IntPtr shr, IntPtr* textures, size_t textureCount, IntPtr* buffers, size_t bufferCount);
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

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate IntPtr sgl_GraphicsDevice_GetPostProcessLayout_ptr(IntPtr device);
        private static sgl_GraphicsDevice_GetPostProcessLayout_ptr sgl_GraphicsDevice_GetPostProcessLayout;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_GraphicsDevice_AddEffect_ptr(IntPtr device, IntPtr effect);
        private static sgl_GraphicsDevice_AddEffect_ptr sgl_GraphicsDevice_AddEffect;

        private readonly List<PostProcess> mPostProcessEffects = [];

        public Colour ClearColour
        {
            set => sgl_GraphicsDevice_SetClearColour(mHandle, value);
        }

        public VertexLayout PostProcessLayout =>
            mPostProcessLayout;

        public void BeginFrame() =>
            sgl_GraphicsDevice_BeginFrame(mHandle);

        public void EndFrame() =>
            sgl_GraphicsDevice_EndFrame(mHandle);

        public void SwapBuffer() =>
            sgl_GraphicsDevice_SwapBuffer(mHandle);

        public void Draw(VertexArray va, Shader shader, Texture[] textures, IUniformBuffer[] uniformBuffers)
        {
            unsafe
            {
                int texCount = textures?.Length ?? 0;
                IntPtr* texturePtrs = stackalloc IntPtr[texCount];

                int bufCount = uniformBuffers?.Length ?? 0;
                IntPtr* bufferPtrs = stackalloc IntPtr[bufCount];

                for (int i = 0; i < texCount; ++i)
                    texturePtrs[i] = textures[i].Handle;

                for (int i = 0; i < bufCount; ++i)
                    bufferPtrs[i] = uniformBuffers[i].Handle;

                sgl_GraphicsDevice_Draw(mHandle, va.Handle, shader.Handle, texturePtrs, (size_t)texCount, bufferPtrs, (size_t)bufCount);
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
            sgl_GraphicsDevice_Create ??= Native.GetFunction<sgl_GraphicsDevice_Create_ptr>();
            sgl_GraphicsDevice_Destroy ??= Native.GetFunction<sgl_GraphicsDevice_Destroy_ptr>();
            sgl_GraphicsDevice_SetClearColour ??= Native.GetFunction<sgl_GraphicsDevice_SetClearColour_ptr>();
            sgl_GraphicsDevice_BeginFrame ??= Native.GetFunction<sgl_GraphicsDevice_BeginFrame_ptr>();
            sgl_GraphicsDevice_EndFrame ??= Native.GetFunction<sgl_GraphicsDevice_EndFrame_ptr>();
            sgl_GraphicsDevice_SwapBuffer ??= Native.GetFunction<sgl_GraphicsDevice_SwapBuffer_ptr>();
            sgl_GraphicsDevice_Draw ??= Native.GetFunction<sgl_GraphicsDevice_Draw_ptr>();
            sgl_GraphicsDevice_GetPostProcessLayout ??= Native.GetFunction<sgl_GraphicsDevice_GetPostProcessLayout_ptr>();
            sgl_GraphicsDevice_AddEffect ??= Native.GetFunction<sgl_GraphicsDevice_AddEffect_ptr>();

            sgl_GraphicsDevice_ImGui_Init ??= Native.GetFunction<sgl_GraphicsDevice_ImGui_Init_ptr>();
            sgl_GraphicsDevice_ImGui_Shutdown ??= Native.GetFunction<sgl_GraphicsDevice_ImGui_Shutdown_ptr>();
            sgl_GraphicsDevice_ImGui_NewFrame ??= Native.GetFunction<sgl_GraphicsDevice_ImGui_NewFrame_ptr>();
            sgl_GraphicsDevice_ImGui_RenderDrawData ??= Native.GetFunction<sgl_GraphicsDevice_ImGui_RenderDrawData_ptr>();
        }

        public void AddEffect(PostProcess effect)
        {
            if (mPostProcessEffects.TryAdd(effect))
                sgl_GraphicsDevice_AddEffect(mHandle, effect.Handle);
        }

        public GraphicsDevice(Window window)
        {
            mHandle = sgl_GraphicsDevice_Create(window.Handle);
            mPostProcessLayout = new(sgl_GraphicsDevice_GetPostProcessLayout(mHandle));
        }

        public void Dispose()
        {
            mPostProcessLayout.Leak();
            mPostProcessLayout.Dispose();

            foreach (PostProcess effect in mPostProcessEffects)
                effect.Dispose();
            mPostProcessEffects.Clear();

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