using SGLNet.Interop;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace SGLNet.Graphics
{
    public unsafe sealed class VertexArray : IDisposable
    {
        private IntPtr mHandle;
        private readonly GraphicsDevice mGraphics;

        private VertexLayout mVertexLayout;

        internal IntPtr Handle =>
            mHandle;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate IntPtr sgl_VertexArray_Create_ptr(IntPtr gpu, uint vertexSize, IntPtr layout);
        private static sgl_VertexArray_Create_ptr sgl_VertexArray_Create;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_VertexArray_Bind_ptr(IntPtr va);
        private static sgl_VertexArray_Bind_ptr sgl_VertexArray_Bind;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_VertexArray_Destroy_ptr(IntPtr va);
        private static sgl_VertexArray_Destroy_ptr sgl_VertexArray_Destroy;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_VertexArray_Set_ptr(IntPtr va, IntPtr vertices, uint vertexCount);
        private static sgl_VertexArray_Set_ptr sgl_VertexArray_Set;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_VertexArray_AddVertex_ptr(IntPtr va, IntPtr vertex);
        private static sgl_VertexArray_AddVertex_ptr sgl_VertexArray_AddVertex;

        public void Bind() =>
            sgl_VertexArray_Bind(mHandle);

        public void Set<T>(ReadOnlySpan<T> vertices) where T : unmanaged
        {
            fixed (T* ptr = vertices)
                sgl_VertexArray_Set(mHandle, (IntPtr)ptr, (uint)vertices.Length);
        }

        public void Add<T>(ref T vertex) where T : unmanaged
        {
            fixed (T* ptr = &vertex)
                sgl_VertexArray_AddVertex(mHandle, (IntPtr)ptr);
        }

        public void AddQuad<T>(in T tl, in T tr, in T bl, in T br) where T : unmanaged
        {
            Add(ref Unsafe.AsRef(in tl));
            Add(ref Unsafe.AsRef(in tr));
            Add(ref Unsafe.AsRef(in br));

            Add(ref Unsafe.AsRef(in tl));
            Add(ref Unsafe.AsRef(in br));
            Add(ref Unsafe.AsRef(in bl));
        }

        public void Dispose()
        {
            sgl_VertexArray_Destroy(mHandle);
            mHandle = IntPtr.Zero;

            mVertexLayout.Dispose();

            GC.SuppressFinalize(this);
        }

        public VertexArray(GraphicsDevice graphics, uint vertexSize, VertexLayout layout)
        {
            sgl_VertexArray_Create ??= Native.GetFunction<sgl_VertexArray_Create_ptr>(nameof(sgl_VertexArray_Create));
            sgl_VertexArray_Bind ??= Native.GetFunction<sgl_VertexArray_Bind_ptr>(nameof(sgl_VertexArray_Bind));
            sgl_VertexArray_Destroy ??= Native.GetFunction<sgl_VertexArray_Destroy_ptr>(nameof(sgl_VertexArray_Destroy));
            sgl_VertexArray_Set ??= Native.GetFunction<sgl_VertexArray_Set_ptr>(nameof(sgl_VertexArray_Set));
            sgl_VertexArray_AddVertex ??= Native.GetFunction<sgl_VertexArray_AddVertex_ptr>(nameof(sgl_VertexArray_AddVertex));

            mVertexLayout = layout.Clone() as VertexLayout;

            mGraphics = graphics;
            mHandle = sgl_VertexArray_Create(graphics.Handle, vertexSize, layout.Handle);
        }

        ~VertexArray()
        {
            Dispose();
        }
    }
}