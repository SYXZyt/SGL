struct VSInput
{
    float3 pos : POSITION0;
    float2 uv : TEXCOORD0;

    float3 instOffset : POSITION1;
    float4 instColour : COLOR0;
    float instScale : TEXCOORD1;
};

struct VSOutput
{
    float4 pos : SV_Position;
    float4 colour : COLOR0;
    float2 uv : TEXCOORD0;
};

struct CameraBuffer
{
    float4x4 view;
    float4x4 proj;
};

[[vk::binding(0)]]
ConstantBuffer<CameraBuffer> gCamera : register(b0);

[[vk::binding(0)]]
Sampler2D gTex : register(t0);

[shader("vertex")]
VSOutput vertexMain(VSInput input)
{
    VSOutput output;

    float3 camRight = float3(gCamera.view[0][0], gCamera.view[0][1], gCamera.view[0][2]);
    float3 camUp = float3(gCamera.view[1][0], gCamera.view[1][1], gCamera.view[1][2]);

    float3 worldPos = input.instOffset + (camRight * input.pos.x + camUp * input.pos.y) * input.instScale;

    output.pos = mul(gCamera.proj, mul(gCamera.view, float4(worldPos, 1.0)));
    output.colour = input.instColour;
    output.uv = input.uv;

    return output;
}

[shader("fragment")]
float4 fragmentMain(VSOutput input) : SV_Target
{
    float4 colour = input.colour * gTex.Sample(input.uv);

    return float4(colour.rgb * colour.a, colour.a);
}