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
layout(location=1) in vec2 aUV;

out vec2 oUV;

layout(std140, binding = 0) uniform CameraBuffer
{
    mat4 view;
    mat4 proj;
};

void main()
{
    gl_Position = proj * view * vec4(aPos, 1.0);
    oUV = aUV;
}
";

        private static readonly string FragmentShaderSourceGL = @"
#version 460 core
in vec2 oUV;
out vec4 FragCol;

layout(binding = 0) uniform sampler2DArray tex;

void main() {
    FragCol = texture(tex, vec3(oUV, 3));
}
";

        private static readonly string VertexShaderSourceDX = @"
struct VSInput
{
    float3 pos : POSITION;
    float2 uv  : TEXCOORD;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD;
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
    output.uv = input.uv;
    return output;
}
";

        private static readonly string PixelShaderSourceDX = @"
struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD;
};

Texture2DArray tex      : register(t0);
SamplerState texSampler : register(s0);

float4 main(PSInput input) : SV_TARGET {
    return tex.Sample(texSampler, float3(input.uv, 3));
}
";

        private static readonly string PostProcessEffectVertexGL = @"
#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;

out vec2 oUV;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    oUV = aUV;
}
";

        private static readonly string PostProcessEffectFragmentGL = @"
#version 460 core
in vec2 oUV;
out vec4 FragCol;

layout(binding = 0) uniform sampler2D frametexture;

void main()
{
    FragCol = texture(frametexture, oUV);
    FragCol.rgb = 1 - FragCol.rgb;
}
";

        private static readonly string PostProcessEffectVertexDX = @"
struct VSInput
{
    float3 Position : POSITION;
    float2 UV : TEXCOORD0;
};

struct VSOutput
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD0;
};

VSOutput main(VSInput input)
{
    VSOutput output;

    output.Position = float4(input.Position, 1.0f);
    output.UV = input.UV;

    return output;
}
";

        private static readonly string PostProcessEffectPixelDX = @"
Texture2D FrameTexture : register(t0);
SamplerState FrameSampler : register(s0);

struct PSInput
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD0;
};

float4 main(PSInput input) : SV_TARGET
{
    float4 colour = FrameTexture.Sample(FrameSampler, input.UV);
    colour.rgb = 1.0f - colour.rgb;
    return colour;
}
";

        [StructLayout(LayoutKind.Explicit)]
        private struct Vertex
        {
            [FieldOffset(0)]
            public Vec3 pos;
            [FieldOffset(12)]
            public Vec2 uv;
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
            //cfg.backend = Backend.OPENGL;

            Runtime.Init();
            Logger.Init();

            Keyboard.Init();

            using (Window window = new(cfg))
            {
                using GraphicsDevice device = new(window);

                using VertexLayout layout = new(device);
                layout.Add(Semantic.POSITION, 0, ElementType.Type.VEC3);
                layout.Add(Semantic.TEXCOORD, 12, ElementType.Type.VEC2);

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

                tl.uv = Vec2.Zero;
                tr.uv = Vec2.Right;
                bl.uv = Vec2.Up;
                br.uv = Vec2.One;

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

                using Texture2DArray texture = new(device, Vec2i.One * 16);
                texture.LoadFromFile("stone.png");

                device.ImGui_Init();

                Vec2 position = Vec2.Zero;

                PostProcess effect = new(device);
                if (cfg.backend == Backend.OPENGL)
                    effect.Shader.LoadFromSource(PostProcessEffectVertexGL, PostProcessEffectFragmentGL);
                else
                    effect.Shader.LoadFromSource(PostProcessEffectVertexDX, PostProcessEffectPixelDX);
                device.AddEffect(effect);

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

                    device.BeginFrame();
                    device.Draw(va, shader, [texture], [ub]);

                    if (ImGui.DragVec2("Position", ref position, 0.1f))
                    {
                        ub.Data.View = Mathf.View(position, 0f);
                        ub.Upload();
                    }

                    device.ImGui_RenderDrawData();

                    device.EndFrame();
                    device.SwapBuffer();
                }

                device.ImGui_Shutdown();
            }

            Keyboard.Shutdown();

            Logger.Shutdown();
            Runtime.Shutdown();
        }
    }
}