using System.Runtime.InteropServices;

namespace SGLNet.Maths
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vec2(float x, float y)
    {
        public static Vec2 Zero =>
            new(0);

        public static Vec2 One =>
            new(1);

        public static Vec2 Left =>
            new(-1, 0);

        public static Vec2 Right =>
            new(1, 0);

        public static Vec2 Up =>
            new(0, 1);

        public static Vec2 Down =>
            new(0, -1);

        private float x = x;
        private float y = y;

        public float X
        {
            readonly get => x;
            set => x = value;
        }

        public float Y
        {
            readonly get => y;
            set => y = value;
        }

        public float U
        {
            readonly get => x;
            set => x = value;
        }

        public float V
        {
            readonly get => y;
            set => y = value;
        }

        public float Width
        {
            readonly get => x;
            set => x = value;
        }

        public float Height
        {
            readonly get => y;
            set => y = value;
        }

        public static Vec2 operator +(Vec2 left, Vec2 right) =>
            new(left.x + right.x, left.y + right.y);

        public static Vec2 operator -(Vec2 left, Vec2 right) =>
            new(left.x - right.x, left.y - right.y);

        public static Vec2 operator *(Vec2 left, Vec2 right) =>
            new(left.x * right.x, left.y * right.y);

        public static Vec2 operator /(Vec2 left, Vec2 right) =>
            new(left.x / right.x, left.y / right.y);

        public static Vec2 operator *(Vec2 v, float scalar) =>
            new(v.x * scalar, v.y * scalar);

        public static Vec2 operator /(Vec2 v, float scalar) =>
            new(v.x / scalar, v.y / scalar);

        public static Vec2 operator -(Vec2 v) =>
            new(-v.x, -v.y);

        public static bool operator ==(Vec2 left, Vec2 right) =>
            left.x == right.x && left.y == right.y;

        public static bool operator !=(Vec2 left, Vec2 right) =>
            !(left == right);

        public override readonly bool Equals(object obj)
        {
            if (obj is null || obj is not Vec2)
                return false;

            return this == (Vec2)obj;
        }

        public override readonly int GetHashCode() =>
            HashCode.Combine(x.GetHashCode(), y.GetHashCode());

        public Vec2() : this(0f) { }

        public Vec2(float scalar) : this(scalar, scalar) { }
    }
}