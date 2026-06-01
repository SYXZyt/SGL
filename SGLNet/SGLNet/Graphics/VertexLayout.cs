using SGLNet.Interop;
using System.Runtime.InteropServices;

namespace SGLNet.Graphics
{
    [StructLayout(LayoutKind.Sequential)]
    public struct ElementType
    {
        public enum Type : byte
        {
            FLOAT,

            INT8,
            INT16,
            INT32,

            UINT8,
            UINT16,
            UINT32,

            VEC2,
            VEC3,
            VEC4,
        }

        public Util.String.NativeString semantic;
        public nuint offset;
        public Type type;
        [MarshalAs(UnmanagedType.U1)] public bool perInstance; // Unused for now
    }

    public sealed class VertexLayout : IDisposable
    {
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate IntPtr sgl_VertexLayout_New_ptr(IntPtr graphicsDevice);
        private static sgl_VertexLayout_New_ptr sgl_VertexLayout_New;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_VertexLayout_Add_ptr(IntPtr layout, ElementType element);
        private static sgl_VertexLayout_Add_ptr sgl_VertexLayout_Add;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_VertexLayout_Destroy_ptr(IntPtr layout);
        private static sgl_VertexLayout_Destroy_ptr sgl_VertexLayout_Destroy;

        private IntPtr mHandle;

        public IntPtr Handle =>
            mHandle;

        internal void Leak()
        {
            mHandle = IntPtr.Zero;
        }

        public void Dispose()
        {
            if (mHandle != IntPtr.Zero)
            {
                sgl_VertexLayout_Destroy(mHandle);
                mHandle = IntPtr.Zero;
            }

            GC.SuppressFinalize(this);
        }

        public void Add(string semantic, int offset, ElementType.Type type)
        {
            ElementType elementType = new()
            {
                semantic = new Util.String(semantic).Handle,
                offset = (nuint)offset,
                type = type,
                perInstance = false
            };

            sgl_VertexLayout_Add(mHandle, elementType);
        }

        public VertexLayout(GraphicsDevice graphics)
        {
            sgl_VertexLayout_New ??= Native.GetFunction<sgl_VertexLayout_New_ptr>(nameof(sgl_VertexLayout_New));
            sgl_VertexLayout_Add ??= Native.GetFunction<sgl_VertexLayout_Add_ptr>(nameof(sgl_VertexLayout_Add));
            sgl_VertexLayout_Destroy ??= Native.GetFunction<sgl_VertexLayout_Destroy_ptr>(nameof(sgl_VertexLayout_Destroy));

            mHandle = sgl_VertexLayout_New(graphics.Handle);
        }

        ~VertexLayout()
        {
            Dispose();
        }
    }
}