using SGLNet.Interop;
using System.Runtime.InteropServices;

namespace SGLNet.Graphics
{
    public readonly struct TextureUnit(uint value) :
        IEquatable<TextureUnit>,
        IComparable<TextureUnit>
    {
        private readonly uint mValue = value;

        public static implicit operator TextureUnit(uint value) =>
            new(value);

        public static implicit operator uint(TextureUnit value) =>
            value.mValue;

        public bool Equals(TextureUnit other) =>
            mValue == other.mValue;

        public override bool Equals(object obj) =>
            obj is TextureUnit other && Equals(other);

        public override int GetHashCode() =>
            mValue.GetHashCode();

        public int CompareTo(TextureUnit other) =>
            mValue.CompareTo(other.mValue);

        public static bool operator ==(TextureUnit left, TextureUnit right) =>
            left.mValue == right.mValue;

        public static bool operator !=(TextureUnit left, TextureUnit right) =>
            left.mValue != right.mValue;

        public static bool operator <(TextureUnit left, TextureUnit right) =>
            left.mValue < right.mValue;

        public static bool operator >(TextureUnit left, TextureUnit right) =>
            left.mValue > right.mValue;

        public static bool operator <=(TextureUnit left, TextureUnit right) =>
            left.mValue <= right.mValue;

        public static bool operator >=(TextureUnit left, TextureUnit right) =>
            left.mValue >= right.mValue;
    }

    public abstract class Texture : IDisposable
    {
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void sgl_Texture_Destroy_ptr(IntPtr handle);
        internal static sgl_Texture_Destroy_ptr sgl_Texture_Destroy;

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        internal delegate void sgl_Texture_Bind_ptr(IntPtr handle, uint unit);
        internal static sgl_Texture_Bind_ptr sgl_Texture_Bind;

        internal static void Init_FuncPtr()
        {
            sgl_Texture_Destroy ??= Native.GetFunction<sgl_Texture_Destroy_ptr>();
            sgl_Texture_Bind ??= Native.GetFunction<sgl_Texture_Bind_ptr>();
        }

        internal abstract IntPtr Handle { get; }

        public void Dispose()
        {
            Destruct();
            GC.SuppressFinalize(this);
        }

        protected abstract void Destruct();

        public abstract void Bind(TextureUnit unit);

        public abstract void LoadFromMemory(byte[] bytes);
        public abstract void LoadFromFile(string path);

        ~Texture() =>
            Dispose();
    }
}