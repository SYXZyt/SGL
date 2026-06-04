using SGLNet.Interop;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace SGLNet.Maths
{
    public static class Mathf
    {
        public const float PI = float.Pi;
        public const float HALF_PI = PI / 2f;
        public const float TWO_PI = PI * 2f;
        public const float TAU = TWO_PI;

        public const float DEG2RAD = PI / 180f;
        public const float RAD2DEG = 180f / PI;

        public const float EPS = float.Epsilon;
        public const float INF = float.PositiveInfinity;
        public const float NEG_INF = float.NegativeInfinity;
        public const float NAN = float.NaN;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool IsPowerOfTwo(int x) =>
            (x > 0) && ((x & (x - 1)) == 0);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Sqrt(float x) =>
            MathF.Sqrt(x);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Sq(float x) =>
            x * x;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool IsMultiple(int value, int multiple) =>
            (value & multiple) == 0;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Lerp(float a, float b, float t) =>
            a * (1f - t) + b * t;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Min(float a, float b) =>
            a < b ? a : b;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Max(float a, float b) =>
            a > b ? a : b;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Clamp(float value, float min, float max) =>
            Max(min, Min(max, value));

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static int Min(int a, int b) =>
            a < b ? a : b;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static int Max(int a, int b) =>
            a > b ? a : b;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static int Clamp(int value, int min, int max) =>
            Max(min, Min(max, value));

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Abs(float f) =>
            f < 0 ? -f : f;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Copysign(float f, int sign) =>
            sign < 0 ? -Abs(f) : Abs(f);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Trunc(float f) =>
            MathF.Truncate(f);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Rad(float deg) =>
            deg * DEG2RAD;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Deg(float rad) =>
            rad * RAD2DEG;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Normalise(float f, float min, float max) =>
            (f - min) / (max - min);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Sin(float f) =>
            MathF.Sin(f);

        public static float Sin(float f, float min, float max)
        {
            float amplitude = (max - min) * 0.5f;
            float mid = (max + min) * 0.5f;
            return Sin(f) * amplitude + mid;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Cos(float f) =>
            MathF.Cos(f);

        public static float Cos(float f, float min, float max)
        {
            float amplitude = (max - min) * 0.5f;
            float mid = (max + min) * 0.5f;
            return Cos(f) * amplitude + mid;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Tan(float f) =>
            MathF.Tan(f);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float ASin(float f) =>
            MathF.Asin(f);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float ACos(float f) =>
            MathF.Acos(f);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float ATan(float f) =>
            MathF.Atan(f);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float ATan2(float y, float x) =>
            MathF.Atan2(y, x);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Log(float f) =>
            MathF.Log(f);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Floor(float f) =>
            MathF.Floor(f);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Ceil(float f) =>
            MathF.Ceiling(f);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Round(float f) =>
            MathF.Round(f);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float Pow(float @base, float exponent) =>
            MathF.Pow(@base, exponent);

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static float LerpDt(float a, float b, float t, float dt) =>
            Lerp(a, b, 1 - Pow(t, dt));

        #region Vec2

        public static float Dist(in Vec2 a, in Vec2 b) =>
            Sqrt(Dist2(a, b));

        public static float Dist2(in Vec2 a, in Vec2 b) =>
            Sq(b.X - a.X) + Sq(b.Y - a.Y);

        public static float Angle(in Vec2 a, in Vec2 b)
        {
            float dot = Dot(a, b);
            float lengths = Length(a) * Length(b);

            return ACos(dot / lengths);
        }

        public static float Direction(in Vec2 v) =>
            ATan2(v.Y, v.X);

        public static float Length(in Vec2 v) =>
            Sqrt(Length2(v));

        public static float Length2(in Vec2 v) =>
            Sq(v.X) + Sq(v.Y);

        public static float Dot(in Vec2 a, in Vec2 b) =>
            a.X * b.X + a.Y * b.Y;

        public static Vec2 Lerp(in Vec2 a, in Vec2 b, float t) =>
            new(Lerp(a.X, b.X, t), Lerp(a.Y, a.Y, t));

        public static Vec2 LerpDt(in Vec2 a, in Vec2 b, float t, float dt) =>
            new(LerpDt(a.X, b.X, t, dt), LerpDt(a.Y, b.Y, t, dt));

        public static Vec2 Normalise(in Vec2 v)
        {
            float len = Length(v);
            if (len == 0f)
                return Vec2.Zero;

            return new(v.X / len, v.Y / len);
        }

        #endregion

        #region Vec2i

        public static float Dist(in Vec2i a, in Vec2i b) =>
            Sqrt(Dist2(a, b));

        public static float Dist2(in Vec2i a, in Vec2i b) =>
            Sq(b.X - a.X) + Sq(b.Y - a.Y);

        public static float Angle(in Vec2i a, in Vec2i b)
        {
            float dot = Dot(a, b);
            float lengths = Length(a) * Length(b);

            return ACos(dot / lengths);
        }

        public static float Direction(in Vec2i v) =>
            ATan2(v.Y, v.X);

        public static float Length(in Vec2i v) =>
            Sqrt(Length2(v));

        public static float Length2(in Vec2i v) =>
            Sq(v.X) + Sq(v.Y);

        public static float Dot(in Vec2i a, in Vec2i b) =>
            a.X * b.X + a.Y * b.Y;

        public static float AspectRatio(in Vec2i v)
        {
            if (v.Y <= 0f)
                return INF;

            return v.Width / v.Height;
        }

        #endregion

        #region Mat4

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate Mat4 sgl_Maths_Mat4_Orthographic_ptr(Vec2i screenSize, float zoom);
        private static sgl_Maths_Mat4_Orthographic_ptr sgl_Maths_Mat4_Orthographic;
        private static sgl_Maths_Mat4_Orthographic_ptr sgl_Maths_Mat4_OrthographicGL;

        public static Mat4 OrthographicGL(in Vec2i screenSize, float zoom) =>
            sgl_Maths_Mat4_OrthographicGL(screenSize, zoom);

        public static Mat4 Orthographic(in Vec2i screenSize, float zoom) =>
            sgl_Maths_Mat4_Orthographic(screenSize, zoom);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate Mat4 sgl_Maths_Mat4_View_ptr(Vec2 position, float angle);
        private static sgl_Maths_Mat4_View_ptr sgl_Maths_Mat4_View;

        public static Mat4 View(in Vec2 position, float angle) =>
            sgl_Maths_Mat4_View(position, angle);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate Mat4 Transpose_Inverse_ptr(Mat4 mat);
        private static Transpose_Inverse_ptr sgl_Maths_Mat4_Inverse;
        private static Transpose_Inverse_ptr sgl_Maths_Mat4_Transpose;

        public static Mat4 Inverse(in Mat4 mat) =>
            sgl_Maths_Mat4_Inverse(mat);

        public static Mat4 Transpose(in Mat4 mat) =>
            sgl_Maths_Mat4_Transpose(mat);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate Mat4 sgl_Maths_Mat4_Translation_ptr(Vec3 translation);
        private static sgl_Maths_Mat4_Translation_ptr sgl_Maths_Mat4_Translation;

        public static Mat4 Translation(in Vec3 translation) =>
            sgl_Maths_Mat4_Translation(translation);

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        private delegate Mat4 sgl_Maths_Mat4_Rotation_ptr(float angle, Vec3 axis);
        private static sgl_Maths_Mat4_Rotation_ptr sgl_Maths_Mat4_Rotation;
        
        public static Mat4 Rotation(float angle, in Vec3 axis) =>
            sgl_Maths_Mat4_Rotation(angle, axis);

        #endregion

        internal static void Init_FuncPtr()
        {
            sgl_Maths_Mat4_OrthographicGL ??= Native.GetFunction<sgl_Maths_Mat4_Orthographic_ptr>(nameof(sgl_Maths_Mat4_OrthographicGL));
            sgl_Maths_Mat4_Orthographic ??= Native.GetFunction<sgl_Maths_Mat4_Orthographic_ptr>(nameof(sgl_Maths_Mat4_Orthographic));
            sgl_Maths_Mat4_View ??= Native.GetFunction<sgl_Maths_Mat4_View_ptr>(nameof(sgl_Maths_Mat4_View));
            sgl_Maths_Mat4_Inverse ??= Native.GetFunction<Transpose_Inverse_ptr>(nameof(sgl_Maths_Mat4_Inverse));
            sgl_Maths_Mat4_Transpose ??= Native.GetFunction<Transpose_Inverse_ptr>(nameof(sgl_Maths_Mat4_Transpose));
            sgl_Maths_Mat4_Translation ??= Native.GetFunction<sgl_Maths_Mat4_Translation_ptr>(nameof(sgl_Maths_Mat4_Translation));
            sgl_Maths_Mat4_Rotation ??= Native.GetFunction<sgl_Maths_Mat4_Rotation_ptr>(nameof(sgl_Maths_Mat4_Rotation));
        }
    }
}