using SGLNet.Graphics;
using SGLNet.Input;
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

layout(std140, binding = 0) uniform CameraBuffer
{
    mat4 view;
    mat4 proj;
};

void main()
{
    gl_Position = proj * view * vec4(aPos, 1.0);
    oCol = aCol;
}
";

        private static readonly string FragmentShaderSourceGL = @"
#version 460 core
in vec3 oCol;
out vec4 FragCol;

void main()
{
    FragCol = vec4(oCol, 1.0);
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

cbuffer CameraMatrices : register(b0)
{
    float4x4 View;
    float4x4 Proj;
}

VSOutput main(VSInput input)
{
    VSOutput output;

    output.pos = mul(Proj, mul(View, float4(input.pos, 1.0f)));
    //output.pos = float4(input.pos, 1);
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

float4 main(PSInput input) : SV_TARGET
{
    return float4(input.col, 1.0f);
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
            public Mat4 View;
            public Mat4 Proj;
        }

        private static void Main(string[] _)
        {
            Native.LoadNative();
            Memory.StackTraceEnableIfDebug();

            EngineConfig cfg = new();
            cfg.enableImGui = true;

            cfg.backend = Backend.DIRECTX11;
            cfg.backend = Backend.OPENGL;

            Runtime.Init();
            Logger.Init();

            Keyboard.Init();

            using (Window window = new(cfg))
            {
                using GraphicsDevice device = new(window);

                using VertexLayout layout = new(device);
                layout.Add(Semantic.POSITION, 0, ElementType.Type.VEC3);
                layout.Add(Semantic.COLOUR, 12, ElementType.Type.VEC3);

                using VertexArray va = new(device, 32, layout);
                
                const float scale = 0.5f;
                const float x = 64f * scale;
                const float y = 64f * scale;

                Vertex tl = new();
                Vertex tr = new();
                Vertex bl = new();
                Vertex br = new();

                tl.pos = new(-x, y, 0);
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
                ub.Data.View = Mathf.View(Vec2.One, 0f);
                ub.Data.Proj = Mathf.OrthographicGL(window.ScreenSize, 1f);

                ub.Upload();

                device.ImGui_Init();

                Vec2 position = Vec2.Zero;

                while (!window.WantClose)
                {
                    {
                        Vec2 movement = Vec2.Zero;

                        if (Keyboard.IsDown(Key.A))
                            movement.X -= 0.1f;
                        if (Keyboard.IsDown(Key.D))
                            movement.X += 0.1f;

                        if (Keyboard.IsDown(Key.W))
                            movement.Y -= 0.1f;
                        if (Keyboard.IsDown(Key.S))
                            movement.Y += 0.1f;

                        if (Mathf.Length(movement) > 0)
                        {
                            movement = Mathf.Normalise(movement);

                            position += movement;
                            ub.Data.View = Mathf.View(position, 0f);
                            ub.Upload();
                        }
                    }

                    window.PollEvents();
                    Keyboard.Update();

                    device.ImGui_NewFrame();

                    device.Clear();
                    device.Draw(va, shader, ub);

                    if (ImGui.DragVec2("Position", ref position, 0.1f))
                    {
                        ub.Data.View = Mathf.View(position, 0f);
                        ub.Upload();
                    }

                    device.ImGui_RenderDrawData();

                    device.Present();
                }

                device.ImGui_Shutdown();
            }

            Keyboard.Shutdown();

            Logger.Shutdown();
            Runtime.Shutdown();
        }
    }
}