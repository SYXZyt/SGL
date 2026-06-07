using SGLNet.Interop;
using System.Runtime.InteropServices;

using static SGLNet.Graphics.UniformBufferNative;

namespace SGLNet.Graphics
{
    internal static class UniformBufferNative
    {
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate IntPtr sgl_UniformBuffer_Create_ptr(IntPtr gpu, nuint size);
        internal static sgl_UniformBuffer_Create_ptr sgl_UniformBuffer_Create;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void sgl_UniformBuffer_Destroy_ptr(IntPtr ub);
        internal static sgl_UniformBuffer_Destroy_ptr sgl_UniformBuffer_Destroy;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void sgl_UniformBuffer_Bind_ptr(IntPtr ub, uint slot);
        internal static sgl_UniformBuffer_Bind_ptr sgl_UniformBuffer_Bind;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal unsafe delegate void sgl_UniformBuffer_Upload_ptr(IntPtr ub, void* data);
        internal static sgl_UniformBuffer_Upload_ptr sgl_UniformBuffer_Upload;

        internal static void Init_FuncPtr()
        {
            sgl_UniformBuffer_Create ??= Native.GetFunction<sgl_UniformBuffer_Create_ptr>();
            sgl_UniformBuffer_Destroy ??= Native.GetFunction<sgl_UniformBuffer_Destroy_ptr>();
            sgl_UniformBuffer_Bind ??= Native.GetFunction<sgl_UniformBuffer_Bind_ptr>();
            sgl_UniformBuffer_Upload ??= Native.GetFunction<sgl_UniformBuffer_Upload_ptr>();
        }
    }

    public sealed class UniformBuffer<T> : IUniformBuffer
        where T : unmanaged
    {
        private IntPtr mHandle;

        private T mData;

        public IntPtr Handle { get => mHandle; }

        public ref T Data =>
            ref mData;

        public bool ImGuiEdit()
        {
            if (ImGuiInspector.Edit(ref mData))
            {
                Upload();
                return true;
            }

            return false;
        }

        public void Dispose()
        {
            if (mHandle != IntPtr.Zero)
            {
                sgl_UniformBuffer_Destroy(mHandle);
                mHandle = IntPtr.Zero;
            }

            GC.SuppressFinalize(this);
        }

        public void Upload()
        {
            unsafe
            {
                fixed (T* ptr = &mData)
                    sgl_UniformBuffer_Upload(mHandle, ptr);
            }
        }

        public void Bind(uint slot) =>
            sgl_UniformBuffer_Bind(mHandle, slot);

        public UniformBuffer(GraphicsDevice device)
        {
            mHandle = sgl_UniformBuffer_Create(device.Handle, (nuint)Marshal.SizeOf<T>());
            mData = new();
        }

        ~UniformBuffer() =>
            Dispose();
    }
}