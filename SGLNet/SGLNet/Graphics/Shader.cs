using SGLNet.Interop;
using System.Runtime.InteropServices;

namespace SGLNet.Graphics
{
    public sealed class Shader : IDisposable
    {
        private IntPtr mHandle;

        internal IntPtr Handle =>
            mHandle;

        private bool mHasLoaded = false;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate IntPtr sgl_Shader_Create_ptr(IntPtr graphics, IntPtr layout);
        private static sgl_Shader_Create_ptr sgl_Shader_Create;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate IntPtr sgl_Shader_Load_Source_ptr(IntPtr shaders, [MarshalAs(UnmanagedType.LPUTF8Str)] string vSrc, [MarshalAs(UnmanagedType.LPUTF8Str)] string fSrc);
        private static sgl_Shader_Load_Source_ptr sgl_Shader_Load_Source;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_Shader_Destroy_ptr(IntPtr shader);
        private static sgl_Shader_Destroy_ptr sgl_Shader_Destroy;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_Shader_Bind_ptr(IntPtr shader);
        private static sgl_Shader_Bind_ptr sgl_Shader_Bind;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        private delegate bool sgl_Shader_Contents_Loaded_ptr(IntPtr shader);
        private static sgl_Shader_Contents_Loaded_ptr sgl_Shader_Contents_Loaded;

        public bool IsLoaded =>
            mHasLoaded;

        public void Bind()
        {
            if (!mHasLoaded)
                throw new("Shader has not been loaded");

            sgl_Shader_Bind(mHandle);
        }

        public void LoadFromSource(string vertexSource, string fragmentSource)
        {
            if (mHasLoaded)
                throw new InvalidOperationException("Shader has already been loaded");

            sgl_Shader_Load_Source(mHandle, vertexSource, fragmentSource);

            mHasLoaded = true;
        }

        public void LoadFromFile(string vertexPath, string fragmentPath)
        {
            if (mHasLoaded)
                throw new InvalidOperationException("Shader has already been loaded");

            if (!File.Exists(vertexPath))
                throw new FileNotFoundException($"Could not find file {vertexPath}");

            if (!File.Exists(fragmentPath))
                throw new FileNotFoundException($"Could not find file {fragmentPath}");

            string vSrc = File.ReadAllText(vertexPath);
            string fSrc = File.ReadAllText(fragmentPath);

            LoadFromSource(vSrc, fSrc);
        }

        public void Dispose()
        {
            sgl_Shader_Destroy(mHandle);
            mHandle = IntPtr.Zero;

            GC.SuppressFinalize(this);
        }

        internal static void Init_FuncPtr()
        {
            sgl_Shader_Create ??= Native.GetFunction<sgl_Shader_Create_ptr>();
            sgl_Shader_Load_Source ??= Native.GetFunction<sgl_Shader_Load_Source_ptr>();
            sgl_Shader_Destroy ??= Native.GetFunction<sgl_Shader_Destroy_ptr>();
            sgl_Shader_Bind ??= Native.GetFunction<sgl_Shader_Bind_ptr>();
            sgl_Shader_Contents_Loaded ??= Native.GetFunction<sgl_Shader_Contents_Loaded_ptr>();
        }

        internal Shader(IntPtr handle)
        {
            mHandle = handle;
            mHasLoaded = sgl_Shader_Contents_Loaded(handle);
        }

        public Shader(GraphicsDevice graphicsDevice, VertexLayout layout)
        {
            mHandle = sgl_Shader_Create(graphicsDevice.Handle, layout.Handle);
        }

        ~Shader() =>
            Dispose();
    }
}