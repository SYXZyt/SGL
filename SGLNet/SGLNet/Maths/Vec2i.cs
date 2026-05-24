using System.Runtime.InteropServices;

namespace SGLNet.Maths
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vec2i(int x, int y)
    {
        public static Vec2i Zero =>
            new(0);

        public static Vec2i One =>
            new(1);

        public static Vec2i Left =>
            new(-1, 0);

        public static Vec2i Right =>
            new(1, 0);

        public static Vec2i Up =>
            new(0, 1);

        public static Vec2i Down =>
            new(0, -1);

        private int x = x;
        private int y = y;

        public int X
        {
            readonly get => x;
            set => x = value;
        }

        public int Y
        {
            readonly get => y;
            set => y = value;
        }

        public int U
        {
            readonly get => x;
            set => x = value;
        }

        public int V
        {
            readonly get => y;
            set => y = value;
        }

        public int Width
        {
            readonly get => x;
            set => x = value;
        }

        public int Height
        {
            readonly get => y;
            set => y = value;
        }

        public static Vec2i operator +(Vec2i left, Vec2i right) =>
            new(left.x + right.x, left.y + right.y);

        public static Vec2i operator -(Vec2i left, Vec2i right) =>
            new(left.x - right.x, left.y - right.y);

        public static Vec2i operator *(Vec2i left, Vec2i right) =>
            new(left.x * right.x, left.y * right.y);

        public static Vec2i operator /(Vec2i left, Vec2i right) =>
            new(left.x / right.x, left.y / right.y);

        public static Vec2i operator *(Vec2i v, int scalar) =>
            new(v.x * scalar, v.y * scalar);

        public static Vec2i operator /(Vec2i v, int scalar) =>
            new(v.x / scalar, v.y / scalar);

        public static Vec2i operator -(Vec2i v) =>
            new(-v.x, -v.y);

        public static bool operator ==(Vec2i left, Vec2i right) =>
            !(left == right);

        public Vec2i() : this(0) { }

        public Vec2i(int scalar) : this(scalar, scalar) { }
    }
}