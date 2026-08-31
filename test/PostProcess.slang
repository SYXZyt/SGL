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

struct FxaaParams
{
    float2 resolution;
    int isActive;
};

[[vk::binding(0)]] Sampler2D gFrame : register(t0);

[[vk::binding(0)]] ConstantBuffer<FxaaParams> gFxaa: register(b0);

[shader("vertex")]
VSOutput vertexMain(VSInput input)
{
    VSOutput output;
    output.pos = float4(input.pos, 1.0);
    output.uv = input.uv;

    return output;
}

static const float FXAA_SPAN_MAX = 8.0;
static const float FXAA_REDUCE_MUL = 1.0 / 8.0;
static const float FXAA_REDUCE_MIN = 1.0 / 128.0;

float Luma(float3 rgb) {
    return dot(rgb, float3(0.299, 0.587, 0.114));
}

float3 Fxaa(float2 uv)
{
    float2 texel = 1.0 / gFxaa.resolution;

    float3 rgbNW = gFrame.Sample(uv + float2(-1.0, -1.0) * texel).rgb;
    float3 rgbNE = gFrame.Sample(uv + float2(1.0, -1.0) * texel).rgb;
    float3 rgbSW = gFrame.Sample(uv + float2(-1.0, 1.0) * texel).rgb;
    float3 rgbSE = gFrame.Sample(uv + float2(1.0, 1.0) * texel).rgb;
    float3 rgbM = gFrame.Sample(uv).rgb;

    float lumaNW = Luma(rgbNW);
    float lumaNE = Luma(rgbNE);
    float lumaSW = Luma(rgbSW);
    float lumaSE = Luma(rgbSE);
    float lumaM = Luma(rgbM);

    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

    float2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y = ((lumaNW + lumaSW) - (lumaNE + lumaSE));

    float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);
    float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);

    dir = clamp(dir * rcpDirMin, float2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX), float2(FXAA_SPAN_MAX, FXAA_SPAN_MAX)) * texel;

    float3 rgbA = 0.5 * (
        gFrame.Sample(uv + dir * (1.0 / 3.0 - 0.5)).rgb +
        gFrame.Sample(uv + dir * (2.0 / 3.0 - 0.5)).rgb
    );

    float3 rgbB = rgbA * 0.5 + 0.25 * (
        gFrame.Sample(uv + dir * -0.5).rgb +
        gFrame.Sample(uv + dir * 0.5).rgb
    );

    float lumaB = Luma(rgbB);

    if (lumaB < lumaMin || lumaB > lumaMax)
    {
        return rgbA;
    }
    else
    {
        return rgbB;
    }
}

[shader("fragment")]
float4 fragmentMain(VSOutput input) : SV_Target
{
    if (gFxaa.isActive != 0)
    {
        float3 rgb = Fxaa(input.uv);
        return float4(rgb, 1);
    }

    float3 rgb = gFrame.Sample(input.uv).rgb;
    return float4(rgb, 1);
}