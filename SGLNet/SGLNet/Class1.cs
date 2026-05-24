using SGLNet.Maths;

namespace SGLNet
{
    internal class Class1
    {
        static void Main(string[] args)
        {
            Vec2 v = new(1, 1);
            Console.WriteLine($"Length : {Mathf.Length(in v)}");
            Console.WriteLine($"Length2: {Mathf.Length2(in v)}");
        }
    }
}