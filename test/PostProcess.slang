struct VSInput
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VSOutput
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
};

struct TimeBuffer
{
    float time;
    float3 _pad;
};

[[vk::binding(0)]] ConstantBuffer<TimeBuffer> gTime : register(b0);

[[vk::binding(0)]] Sampler2D gFrame : register(t0);

[shader("vertex")]
VSOutput vertexMain(VSInput input)
{
    VSOutput output;
    output.pos = float4(input.pos, 1.0);
    output.uv = input.uv;

    return output;
}

[shader("fragment")]
float4 fragmentMain(VSOutput input) : SV_Target
{
    float4 colour = gFrame.Sample(input.uv);
    float3 rgb = colour.rgb;
    float3 invRgb = 1.0 - rgb;
    float3 finalColour = lerp(rgb, invRgb, sin(gTime.time * 0.1));

    return float4(finalColour, 1);
}