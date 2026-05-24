using SGLNet.Maths;
using System.Runtime.InteropServices;

namespace SGLNet.Interop
{
    public partial class MathsInterop
    {
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate float sgl_Maths_Length_ptr(Vec2 v);
        private static sgl_Maths_Length_ptr sgl_Maths_Length;
        public static float Length(in Vec2 v)
        {
            sgl_Maths_Length ??= Native.GetFunction<sgl_Maths_Length_ptr>(nameof(sgl_Maths_Length));
            return sgl_Maths_Length(v);
        }

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate float sgl_Maths_Length2_ptr(Vec2 v);
        private static sgl_Maths_Length2_ptr sgl_Maths_Length2;
        public static float Length2(in Vec2 v)
        {
            sgl_Maths_Length2 ??= Native.GetFunction<sgl_Maths_Length2_ptr>(nameof(sgl_Maths_Length2));
            return sgl_Maths_Length2(v);
        }
    }
}