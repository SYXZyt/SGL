using SGLNet.Util;

namespace SGLNet
{
    internal class Class1
    {
        static void Main(string[] _)
        {
            EngineConfig cfg = new();

            Runtime.Init();
            Logger.Init();

            using (Window window = new(cfg))
            {
                window.Resize += (w, h) => { Console.WriteLine($"Resize: {w}x{h}"); };

                while (!window.WantClose)
                {
                    window.PollEvents();
                    window.SwapBuffer();
                }
            }

            Logger.Shutdown();
            Runtime.Shutdown();
        }
    }
}