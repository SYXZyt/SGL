using System.Runtime.InteropServices;

namespace SGLNet.Maths
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Mat4
    {
        public float M00, M10, M20, M30;
        public float M01, M11, M21, M31;
        public float M02, M12, M22, M32;
        public float M03, M13, M23, M33;

        public static Mat4 Identity =>
            new(1.0f);

        public Mat4(float diagonal)
        {
            M00 = diagonal;
            M01 = 0.0f;
            M02 = 0.0f;
            M03 = 0.0f;

            M10 = 0.0f;
            M11 = diagonal;
            M12 = 0.0f;
            M13 = 0.0f;

            M20 = 0.0f;
            M21 = 0.0f;
            M22 = diagonal;
            M23 = 0.0f;

            M30 = 0.0f;
            M31 = 0.0f;
            M32 = 0.0f;
            M33 = diagonal;
        }

        public float this[int row, int column]
        {
            readonly get
            {
                return (row, column) switch
                {
                    (0, 0) => M00,
                    (0, 1) => M01,
                    (0, 2) => M02,
                    (0, 3) => M03,

                    (1, 0) => M10,
                    (1, 1) => M11,
                    (1, 2) => M12,
                    (1, 3) => M13,

                    (2, 0) => M20,
                    (2, 1) => M21,
                    (2, 2) => M22,
                    (2, 3) => M23,

                    (3, 0) => M30,
                    (3, 1) => M31,
                    (3, 2) => M32,
                    (3, 3) => M33,

                    _ => throw new IndexOutOfRangeException()
                };
            }

            set
            {
                switch ((row, column))
                {
                    case (0, 0): M00 = value; break;
                    case (0, 1): M01 = value; break;
                    case (0, 2): M02 = value; break;
                    case (0, 3): M03 = value; break;

                    case (1, 0): M10 = value; break;
                    case (1, 1): M11 = value; break;
                    case (1, 2): M12 = value; break;
                    case (1, 3): M13 = value; break;

                    case (2, 0): M20 = value; break;
                    case (2, 1): M21 = value; break;
                    case (2, 2): M22 = value; break;
                    case (2, 3): M23 = value; break;

                    case (3, 0): M30 = value; break;
                    case (3, 1): M31 = value; break;
                    case (3, 2): M32 = value; break;
                    case (3, 3): M33 = value; break;

                    default:
                        throw new IndexOutOfRangeException();
                }
            }
        }
    }
}