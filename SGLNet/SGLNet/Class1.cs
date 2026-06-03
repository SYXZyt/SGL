using SGLNet.Graphics;
using SGLNet.Interop;
using SGLNet.Maths;
using SGLNet.Util;
using System.Runtime.InteropServices;

namespace SGLNet
{
    internal class Class1
    {
        private static readonly string VertexShaderSourceGL = @"
#version 460 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aCol;

out vec3 oCol;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    oCol = aCol;
}
";

        private static readonly string FragmentShaderSourceGL = @"
#version 460 core
in vec3 oCol;
out vec4 FragCol;

layout(std140, binding = 0) uniform ColourBuffer {
    vec3 c;
};

void main()
{
    vec3 a = oCol;
    vec3 b = c;
    vec3 c = mix(a, b, 0.5);

    FragCol = vec4(c, 1.0);
}
";

        private static readonly string VertexShaderSourceDX = @"
struct VSInput
{
    float3 pos : POSITION;
    float3 col : COLOR;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    float3 col : COLOR;
};

VSOutput main(VSInput input)
{
    VSOutput output;

    output.pos = float4(input.pos, 1.0f);
    output.col = input.col;
    return output;
}
";

        private static readonly string PixelShaderSourceDX = @"
struct PSInput
{
    float4 pos : SV_POSITION;
    float3 col : COLOR;
};

cbuffer ColourBuffer : register(b0) {
    float3 c;
};

float4 main(PSInput input) : SV_TARGET
{
    float3 a = input.col;
    float3 b = c;
    float3 c = lerp(a, b, 0.5);

    return float4(c, 1.0f);
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

        [StructLayout(LayoutKind.Sequential, Pack = 16)]
        private struct UB
        {
            public Vec3 colour;
            private float p;
        }

        private static void Main(string[] _)
        {
            Native.LoadNative();
            Memory.StackTraceEnableIfDebug();

            EngineConfig cfg = new();
            cfg.backend = Backend.DIRECTX11;
            cfg.backend = Backend.OPENGL;

            Runtime.Init();
            Logger.Init();

            using (Window window = new(cfg))
            {
                using GraphicsDevice device = new(window);

                int u = Marshal.SizeOf<Vertex>();

                using VertexLayout layout = new(device);
                layout.Add(Semantic.POSITION, 0, ElementType.Type.VEC3);
                layout.Add(Semantic.COLOUR, 12, ElementType.Type.VEC3);

                using VertexArray va = new(device, 32, layout);

                const float scale = 2.8f;
                const float x = 0.1333f * scale;
                const float y = 0.2370f * scale;

                Vertex tl = new();
                Vertex tr = new();
                Vertex bl = new();
                Vertex br = new();

                tl.pos = new(-x * 1.5f, y, 0);
                tr.pos = new(x, y, 0);
                br.pos = new(x, -y, 0);
                bl.pos = new(-x, -y, 0);

                tl.col = Colour.Red;
                tr.col = Colour.Green;
                bl.col = Colour.Blue;
                br.col = Colour.Yellow;

                va.AddQuad(in tl, in tr, in bl, in br);

                using Shader shader = new(device, layout);

                if (cfg.backend == Backend.OPENGL)
                    shader.LoadFromSource(VertexShaderSourceGL, FragmentShaderSourceGL);
                else
                    shader.LoadFromSource(VertexShaderSourceDX, PixelShaderSourceDX);

                window.Resize += (w, h) => { Console.WriteLine($"Resize: {w}x{h}"); };

                using UniformBuffer<UB> ub = new(device);

                float h = 0f;

                while (!window.WantClose)
                {
                    ub.Data.colour = Colour.FromHSV(h, 1, 1);
                    ub.Upload();
                    h += 0.001f;

                    window.PollEvents();

                    device.Clear();

                    device.Draw(va, shader, ub);

                    device.Present();
                }
            }

            Logger.Shutdown();
            Runtime.Shutdown();
        }
    }
}