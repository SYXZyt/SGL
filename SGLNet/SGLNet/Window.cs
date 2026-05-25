using SGLNet.Interop;
using SGLNet.Maths;
using System;
using System.Runtime.InteropServices;

namespace SGLNet
{
    public sealed class Window : IDisposable
    {
        private EngineConfig mConfig;
        private IntPtr mNativeHandle;
        private GCHandle mSelfHandle;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate IntPtr sgl_Window_Create_ptr(EngineConfig config);
        private static sgl_Window_Create_ptr sgl_Window_Create;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_Window_Destroy_ptr(IntPtr window);
        private static sgl_Window_Destroy_ptr sgl_Window_Destroy;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_Window_PollEvents_ptr(IntPtr window);
        private static sgl_Window_PollEvents_ptr sgl_Window_PollEvents;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        private delegate bool sgl_Window_WantClose_ptr(IntPtr window);
        private static sgl_Window_WantClose_ptr sgl_Window_WantClose;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_Window_ResizeCallback_ptr(IntPtr window, Vec2i size, IntPtr userdata);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_Window_RegisterResize_ptr(IntPtr window, sgl_Window_ResizeCallback_ptr callback, IntPtr userdata);
        private static sgl_Window_RegisterResize_ptr sgl_Window_RegisterResize;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_Window_DeregisterResize_ptr(IntPtr window, sgl_Window_ResizeCallback_ptr callback);
        private static sgl_Window_DeregisterResize_ptr sgl_Window_DeregisterResize;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate void sgl_Window_SwapBuffer_ptr(IntPtr window);

        private static sgl_Window_SwapBuffer_ptr sgl_Window_SwapBuffer;
        
        public event Action<int, int> Resize;

        public bool WantClose =>
            sgl_Window_WantClose(mNativeHandle);

        public void PollEvents() =>
            sgl_Window_PollEvents(mNativeHandle);

        public void SwapBuffer() =>
        sgl_Window_SwapBuffer(mNativeHandle);
        
        public Window(EngineConfig cfg)
        {
            sgl_Window_Create ??= Native.GetFunction<sgl_Window_Create_ptr>(nameof(sgl_Window_Create));
            sgl_Window_Destroy ??= Native.GetFunction<sgl_Window_Destroy_ptr>(nameof(sgl_Window_Destroy));
            sgl_Window_PollEvents ??= Native.GetFunction<sgl_Window_PollEvents_ptr>(nameof(sgl_Window_PollEvents));
            sgl_Window_WantClose ??= Native.GetFunction<sgl_Window_WantClose_ptr>(nameof(sgl_Window_WantClose));
            sgl_Window_RegisterResize ??= Native.GetFunction<sgl_Window_RegisterResize_ptr>(nameof(sgl_Window_RegisterResize));
            sgl_Window_DeregisterResize ??= Native.GetFunction<sgl_Window_DeregisterResize_ptr>(nameof(sgl_Window_DeregisterResize));
            sgl_Window_SwapBuffer ??= Native.GetFunction<sgl_Window_SwapBuffer_ptr>(nameof(sgl_Window_SwapBuffer));

            mConfig = cfg;
            mNativeHandle = sgl_Window_Create(cfg);

            mSelfHandle = GCHandle.Alloc(this);

            sgl_Window_RegisterResize(
                mNativeHandle,
                OnResize,
                GCHandle.ToIntPtr(mSelfHandle));
        }
        
        private static void OnResize(IntPtr window, Vec2i size, IntPtr userdata)
        {
            GCHandle handle = GCHandle.FromIntPtr(userdata);

            if (handle.Target is not Window windowInstance)
                return;

            windowInstance.Resize?.Invoke(size.X, size.Y);
        }

        public void Dispose()
        {
            sgl_Window_DeregisterResize(mNativeHandle, OnResize);

            if (mNativeHandle != IntPtr.Zero)
            {
                sgl_Window_Destroy(mNativeHandle);
                mNativeHandle = IntPtr.Zero;
            }

            if (mSelfHandle.IsAllocated)
            {
                mSelfHandle.Free();
            }

            GC.SuppressFinalize(this);
        }

        ~Window() =>
            Dispose();
    }
}