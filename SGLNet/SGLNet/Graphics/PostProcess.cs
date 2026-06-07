using SGLNet.Interop;
using System.Runtime.InteropServices;

namespace SGLNet.Graphics
{
    public sealed class PostProcess : IDisposable, IImGuiEdit
    {
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate IntPtr sgl_PostProcess_Create_ptr(IntPtr device, size_t uniformCount);
        private static sgl_PostProcess_Create_ptr sgl_PostProcess_Create;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_PostProcess_AddUniformBuffer_ptr(IntPtr pp, IntPtr buffer, size_t index);
        private static sgl_PostProcess_AddUniformBuffer_ptr sgl_PostProcess_AddUniformBuffer;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_PostProcess_Destroy_ptr(IntPtr pp);
        private static sgl_PostProcess_Destroy_ptr sgl_PostProcess_Destroy;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate IntPtr sgl_PostProcess_GetShader_ptr(IntPtr pp);
        private static sgl_PostProcess_GetShader_ptr sgl_PostProcess_GetShader;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_PostProcess_SetEnabled_ptr(IntPtr pp, [MarshalAs(UnmanagedType.U1)] bool enabled);
        private static sgl_PostProcess_SetEnabled_ptr sgl_PostProcess_SetEnabled;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        private delegate bool sgl_PostProcess_GetEnabled_ptr(IntPtr pp);
        private static sgl_PostProcess_GetEnabled_ptr sgl_PostProcess_GetEnabled;

        private readonly List<IUniformBuffer> mUniforms = [];

        internal static void Init_FuncPtr()
        {
            sgl_PostProcess_Create ??= Native.GetFunction<sgl_PostProcess_Create_ptr>();
            sgl_PostProcess_Destroy ??= Native.GetFunction<sgl_PostProcess_Destroy_ptr>();
            sgl_PostProcess_GetShader ??= Native.GetFunction<sgl_PostProcess_GetShader_ptr>();
            sgl_PostProcess_AddUniformBuffer ??= Native.GetFunction<sgl_PostProcess_AddUniformBuffer_ptr>();
            sgl_PostProcess_SetEnabled ??= Native.GetFunction<sgl_PostProcess_SetEnabled_ptr>();
            sgl_PostProcess_GetEnabled ??= Native.GetFunction<sgl_PostProcess_GetEnabled_ptr>();
        }

        private IntPtr mHandle;
        internal IntPtr Handle =>
            mHandle;

        private readonly Shader mShader;
        public Shader Shader =>
            mShader;

        public bool Enabled
        {
            get => sgl_PostProcess_GetEnabled(mHandle);
            set => sgl_PostProcess_SetEnabled(mHandle, value);
        }

        public void Dispose()
        {
            sgl_PostProcess_Destroy(mHandle);
            mHandle = IntPtr.Zero;

            GC.SuppressFinalize(this);
        }

        public void AddUniforms(IUniformBuffer uniform)
        {
            if (mUniforms.TryAdd(uniform))
                sgl_PostProcess_AddUniformBuffer(mHandle, uniform.Handle, (size_t)mUniforms.FindIndex(u => u == uniform));
        }

        void IImGuiEdit.Edit()
        {
            bool enabled = Enabled;
            if (ImGui.Checkbox("Enabled", ref enabled))
                Enabled = enabled;

            foreach (IUniformBuffer buffer in mUniforms)
                buffer.ImGuiEdit();
        }

        public PostProcess(GraphicsDevice device, uint uniformCount)
        {
            mHandle = sgl_PostProcess_Create(device.Handle, uniformCount);
            mShader = new(sgl_PostProcess_GetShader(mHandle));
        }

        ~PostProcess() =>
            Dispose();
    }
}