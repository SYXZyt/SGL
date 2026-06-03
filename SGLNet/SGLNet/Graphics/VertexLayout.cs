using SGLNet.Interop;
using System.Runtime.InteropServices;

namespace SGLNet.Graphics
{
    public enum Semantic : byte
    {
        POSITION,
        COLOUR,
        TEXCOORD,
    }

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

        public byte semantic;
        public nuint offset;
        public Type type;
        [MarshalAs(UnmanagedType.U1)] public bool perInstance; // Unused for now
    }

    public sealed class VertexLayout : IDisposable, ICloneable
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

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate IntPtr sgl_VertexLayout_DeepCopy_ptr(IntPtr layout);
        private static sgl_VertexLayout_DeepCopy_ptr sgl_VertexLayout_DeepCopy;

        private IntPtr mHandle;

        public IntPtr Handle =>
            mHandle;

        public void Dispose()
        {
            if (mHandle != IntPtr.Zero)
            {
                sgl_VertexLayout_Destroy(mHandle);
                mHandle = IntPtr.Zero;
            }

            GC.SuppressFinalize(this);
        }

        public void Add(Semantic semantic, int offset, ElementType.Type type)
        {
            ElementType elementType = new()
            {
                semantic = (byte)semantic,
                offset = (nuint)offset,
                type = type,
                perInstance = false
            };

            sgl_VertexLayout_Add(mHandle, elementType);
        }

        public object Clone()
        {
            VertexLayout clone = new() {
                mHandle = sgl_VertexLayout_DeepCopy(mHandle)
            };

            return clone;
        }

        internal static void Init_FuncPtr()
        {
            sgl_VertexLayout_New ??= Native.GetFunction<sgl_VertexLayout_New_ptr>(nameof(sgl_VertexLayout_New));
            sgl_VertexLayout_Add ??= Native.GetFunction<sgl_VertexLayout_Add_ptr>(nameof(sgl_VertexLayout_Add));
            sgl_VertexLayout_Destroy ??= Native.GetFunction<sgl_VertexLayout_Destroy_ptr>(nameof(sgl_VertexLayout_Destroy));
            sgl_VertexLayout_DeepCopy ??= Native.GetFunction<sgl_VertexLayout_DeepCopy_ptr>(nameof(sgl_VertexLayout_DeepCopy));
        }

        private VertexLayout()
        {
            mHandle = IntPtr.Zero;
        }

        public VertexLayout(GraphicsDevice graphics)
        {
            mHandle = sgl_VertexLayout_New(graphics.Handle);
        }

        ~VertexLayout()
        {
            Dispose();
        }
    }
}