using System.Runtime.InteropServices;

namespace SGLNet.Maths
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vec3(float x, float y, float z)
    {
        public static Vec3 Zero =>
            new(0);

        public static Vec3 One =>
            new(1);

        public static Vec3 Left =>
            new(-1, 0, 0);

        public static Vec3 Right =>
            new(1, 0, 0);

        public static Vec3 Up =>
            new(0, 1, 0);

        public static Vec3 Down =>
            new(0, -1, 0);

        public static Vec3 Forward =>
            new(0, 0, 1);

        public static Vec3 Backwards =>
            new(0, 0, -1);

        private float x = x;
        private float y = y;
        private float z = z;

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

        public static Vec3 operator +(Vec3 left, Vec3 right) =>
            new(left.x + right.x, left.y + right.y, left.z + right.z);

        public static Vec3 operator -(Vec3 left, Vec3 right) =>
            new(left.x - right.x, left.y - right.y, left.z - right.z);

        public static Vec3 operator *(Vec3 left, Vec3 right) =>
            new(left.x * right.x, left.y * right.y, left.z * right.z);

        public static Vec3 operator /(Vec3 left, Vec3 right) =>
            new(left.x / right.x, left.y / right.y, left.z / right.z);

        public static Vec3 operator *(Vec3 v, float scalar) =>
            new(v.x * scalar, v.y * scalar, v.z * scalar);

        public static Vec3 operator /(Vec3 v, float scalar) =>
            new(v.x / scalar, v.y / scalar, v.z / scalar);

        public static Vec3 operator -(Vec3 v) =>
            new(-v.x, -v.y, -v.z);

        public static bool operator ==(Vec3 left, Vec3 right) =>
            left.x == right.x && left.y == right.y && left.z == right.z;

        public static bool operator !=(Vec3 left, Vec3 right) =>
            !(left == right);

        public override readonly bool Equals(object obj)
        {
            if (obj is null || obj is not Vec3)
                return false;

            return this == (Vec3)obj;
        }

        public override readonly int GetHashCode() =>
            HashCode.Combine(x.GetHashCode(), y.GetHashCode());

        public Vec3() : this(0f) { }

        public Vec3(float scalar) : this(scalar, scalar, scalar) { }
    }
}