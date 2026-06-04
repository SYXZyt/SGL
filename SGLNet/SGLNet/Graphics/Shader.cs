using SGLNet.Interop;
using System.Runtime.InteropServices;

namespace SGLNet.Graphics
{
    public sealed class Shader : IDisposable
    {
        private IntPtr mHandle;

        internal IntPtr Handle =>
            mHandle;

        private readonly GraphicsDevice mGraphics;

        private readonly VertexLayout mLayout;

        private bool mHasLoaded = false;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate IntPtr sgl_Shader_Create_Source_ptr(IntPtr graphics, [MarshalAs(UnmanagedType.LPUTF8Str)] string vSrc, [MarshalAs(UnmanagedType.LPUTF8Str)] string fSrc, IntPtr layout);
        private static sgl_Shader_Create_Source_ptr sgl_Shader_Create_Source;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_Shader_Destroy_ptr(IntPtr shader);
        private static sgl_Shader_Destroy_ptr sgl_Shader_Destroy;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_Shader_Bind_ptr(IntPtr shader);
        private static sgl_Shader_Bind_ptr sgl_Shader_Bind;

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
            if (mHandle != IntPtr.Zero)
                throw new InvalidOperationException("Shader has already been created");

            mHandle = sgl_Shader_Create_Source(mGraphics.Handle, vertexSource, fragmentSource, mLayout.Handle);

            mHasLoaded = true;
        }

        public void LoadFromFile(string vertexPath, string fragmentPath)
        {
            if (mHandle != IntPtr.Zero)
                throw new InvalidOperationException("Shader has already been created");

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
            mLayout.Dispose();

            GC.SuppressFinalize(this);
        }

        internal static void Init_FuncPtr()
        {
            sgl_Shader_Create_Source ??= Native.GetFunction<sgl_Shader_Create_Source_ptr>(nameof(sgl_Shader_Create_Source));
            sgl_Shader_Destroy ??= Native.GetFunction<sgl_Shader_Destroy_ptr>(nameof(sgl_Shader_Destroy));
            sgl_Shader_Bind ??= Native.GetFunction<sgl_Shader_Bind_ptr>(nameof(sgl_Shader_Bind));
        }

        public Shader(GraphicsDevice graphicsDevice, VertexLayout layout)
        {
            mGraphics = graphicsDevice;
            mLayout = layout.Clone() as VertexLayout;
        }

        ~Shader() =>
            Dispose();
    }
}