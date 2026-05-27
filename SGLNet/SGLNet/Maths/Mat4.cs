using System.Runtime.InteropServices;

namespace SGLNet.Maths
{
    [StructLayout(LayoutKind.Sequential)]
    public readonly struct Mat4
    {
        public static Mat4 Identity =>
            new(1f);

        private readonly float[] values = new float[16];

        public readonly float[] Values =>
            values;

        public readonly float this[int idx]
        {
            get => values[idx];
            set => values[idx] = value;
        }

        public readonly float this[int row, int column]
        {
            get => (row * 16) + column;
            set => values[(row * 16) + column] = value;
        }

        public Mat4() :
            this(1f) {
        }

        public Mat4(float diagonal)
        {
            for (int i = 0; i < 4; ++i)
                this[i, i] = diagonal;
        }
    }
}