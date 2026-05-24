using SGLNet.Util;

namespace SGLNet
{
    internal class Class1
    {
        static void Main(string[] _)
        {
            Runtime.Init();
            Logger.Init();
            
            Logger.Shutdown();
            Runtime.Shutdown();
        }
    }
}