namespace SGLNet.Maths
{
    public static class Mathf
    {
        public static float Length(in Vec2 v) =>
            Interop.MathsInterop.Length(in v);

        public static float Length2(in Vec2 v) =>
            Interop.MathsInterop.Length2(in v);
    }
}