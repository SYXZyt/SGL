struct VSInput
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct VSOutput
{
    float4 pos : SV_Position;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct CameraBuffer
{
    float4x4 view;
    float4x4 proj;
};

struct LightBuffer
{
    float3 lightDir;
    float ambient;
    float3 lightColour;
    float _pad;
};

[[vk::binding(0)]]
ConstantBuffer<CameraBuffer> gCamera : register(b0);

[[vk::binding(1)]]
ConstantBuffer<LightBuffer> gLight : register(b1);

[[vk::binding(0)]]
Sampler2D gTex : register(t0);

[shader("vertex")]
VSOutput vertexMain(VSInput input)
{
    VSOutput output;
    output.pos = mul(gCamera.proj, mul(gCamera.view, float4(input.pos, 1.0)));
    output.normal = input.normal;
    output.uv = input.uv;

    return output;
}

[shader("fragment")]
float4 fragmentMain(VSOutput input) : SV_Target
{
    float3 n = normalize(input.normal);
    float ndotl = max(dot(n, normalize(gLight.lightDir)), 0.0);
    float3 lighting = gLight.lightColour * (gLight.ambient + (1.0 - gLight.ambient) * ndotl);
    float4 texColour = gTex.Sample(input.uv);

    return float4(texColour.rgb * lighting, texColour.a);
}