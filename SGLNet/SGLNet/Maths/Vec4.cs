using System.Runtime.InteropServices;

namespace SGLNet.Maths
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vec4(float x, float y, float z, float w)
    {
        public static Vec4 Zero =>
            new(0);

        public static Vec4 One =>
            new(1);

        public static Vec4 Left =>
            new(-1, 0, 0, 0);

        public static Vec4 Right =>
            new(1, 0, 0, 0);

        public static Vec4 Up =>
            new(0, 1, 0, 0);

        public static Vec4 Down =>
            new(0, -1, 0, 0);

        public static Vec4 Forward =>
            new(0, 0, 1, 0);

        public static Vec4 Backwards =>
            new(0, 0, -1, 0);

        private float x = x;
        private float y = y;
        private float z = z;
        private float w = w;

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

        public float Z
        {
            readonly get => z;
            set => z = value;
        }

        public float W
        {
            readonly get => w;
            set => z = value;
        }

        public static Vec4 operator +(Vec4 left, Vec4 right) =>
            new(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w);

        public static Vec4 operator -(Vec4 left, Vec4 right) =>
            new(left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w);

        public static Vec4 operator *(Vec4 left, Vec4 right) =>
            new(left.x * right.x, left.y * right.y, left.z * right.z, left.w * right.w);

        public static Vec4 operator /(Vec4 left, Vec4 right) =>
            new(left.x / right.x, left.y / right.y, left.z / right.z, left.w / right.w);

        public static Vec4 operator *(Vec4 v, float scalar) =>
            new(v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar);

        public static Vec4 operator /(Vec4 v, float scalar) =>
            new(v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar);

        public static Vec4 operator -(Vec4 v) =>
            new(-v.x, -v.y, -v.z, -v.w);

        public static bool operator ==(Vec4 left, Vec4 right) =>
            left.x == right.x && left.y == right.y && left.z == right.z && left.w == right.w;

        public static bool operator !=(Vec4 left, Vec4 right) =>
            !(left == right);

        public override readonly bool Equals(object obj)
        {
            if (obj is null || obj is not Vec4)
                return false;

            return this == (Vec4)obj;
        }

        public override readonly int GetHashCode() =>
            HashCode.Combine(x.GetHashCode(), y.GetHashCode());

        public Vec4() : this(0f) { }

        public Vec4(float scalar) : this(scalar, scalar, scalar, scalar) { }
    }
}