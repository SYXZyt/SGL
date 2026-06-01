using SGLNet.Graphics;
using SGLNet.Maths;
using SGLNet.Util;
using System.Runtime.InteropServices;

namespace SGLNet
{
    internal class Class1
    {
        private static string VertexShaderSource = @"
#version 330
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aCol;

out vec3 oCol;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    oCol = aCol;
}
";

        private static string FragmentShaderSource = @"
#version 330
in vec3 oCol;
out vec4 FragCol;

void main()
{
    FragCol = vec4(oCol, 1.0);
}
";

        [StructLayout(LayoutKind.Explicit)]
        private struct Vertex
        {
            [FieldOffset(0)]
            public Vec3 pos;
            [FieldOffset(12)]
            public Vec3 col;
        }

        private static void Main(string[] _)
        {
            EngineConfig cfg = new();
            
            Runtime.Init();
            Logger.Init();

            using (Window window = new(cfg))
            {
                using GraphicsDevice device = new(window);

                int u = Marshal.SizeOf<Vertex>();

                using VertexLayout layout = new(device);
                layout.Add("POSITION", 0, ElementType.Type.VEC3);
                layout.Add("COLOR0", 12, ElementType.Type.VEC3);

                using VertexArray va = new(device, 32, layout);

                const float scale = 2.8f;
                const float x = 0.1333f * scale;
                const float y = 0.2370f * scale;

                Vertex tl = new();
                Vertex tr = new();
                Vertex bl = new();
                Vertex br = new();

                tl.pos = new(-x * 1.5f, y, 0);
                tr.pos = new( x, y, 0);
                br.pos = new( x,  -y, 0);
                bl.pos = new(-x,  -y, 0);

                tl.col = Colour.Red;
                tr.col = Colour.Green;
                bl.col = Colour.Blue;
                br.col = Colour.Yellow;

                va.AddQuad(in tl, in tr, in bl, in br);

                using Shader shader = new(device);
                shader.LoadFromSource(VertexShaderSource, FragmentShaderSource);

                window.Resize += (w, h) => { Console.WriteLine($"Resize: {w}x{h}"); };

                while (!window.WantClose)
                {
                    window.PollEvents();

                    device.Clear();

                    device.Draw(va, shader);

                    device.Present();
                }
            }

            Logger.Shutdown();
            Runtime.Shutdown();
        }
    }
}