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

static const float EdgeThresholdMin = 0.0312;
static const float EdgeThreshold = 0.125;
static const float SubpixTrim = 0.25;
static const float SubpixTrimScale = 1.0 / (1.0 - SubpixTrim);
static const float SubpixCap = 0.75;
static const float SearchSteps = 10;

float Luma(float3 rgb) {
    return dot(rgb, float3(0.299, 0.587, 0.114));
}

float SampleLuma(float2 uv) {
    return Luma(gFrame.SampleLevel(uv, 0).rgb);
}

[shader("fragment")]
float4 fragmentMain(VSOutput input) : SV_Target
{
    float2 texelSize = 1.0 / gFxaa.resolution;

    float2 uv = input.uv;
    float3 centreColour = gFrame.SampleLevel(uv, 0).rgb;

    if (gFxaa.isActive == 0)
        return float4(centreColour, 1);

    float lumaCentre = SampleLuma(uv);

    float lumaN  = SampleLuma(uv + float2(0.0, 1.0) * texelSize);
    float lumaS  = SampleLuma(uv + float2(0.0, -1.0) * texelSize);
    float lumaE  = SampleLuma(uv + float2(1.0, 0.0) * texelSize);
    float lumaW  = SampleLuma(uv + float2(-1.0, 0.0) * texelSize);
    float lumaNE = SampleLuma(uv + float2(1.0, 1.0) * texelSize);
    float lumaNW = SampleLuma(uv + float2(-1.0, 1.0) * texelSize);
    float lumaSE = SampleLuma(uv + float2(1.0, -1.0) * texelSize);
    float lumaSW = SampleLuma(uv + float2(-1.0, -1.0) * texelSize);

    float lumaMin = min(lumaCentre, min(min(lumaN, lumaS), min(lumaE, lumaW)));
    float lumaMax = max(lumaCentre, max(max(lumaN, lumaS), max(lumaE, lumaW)));
    float range = lumaMax - lumaMin;

    if (range < max(EdgeThresholdMin, lumaMax * EdgeThreshold))
        return float4(centreColour, 1.0);

    float lumaAvgFull = 2.0 * (lumaN + lumaS + lumaE + lumaW) +
                         (lumaNE + lumaNW + lumaSE + lumaSW);
    lumaAvgFull *= 1.0 / 12.0;
    float subpixContrast = abs(lumaAvgFull - lumaCentre) / range;
    float subpixBlend = max(0.0, subpixContrast - SubpixTrim) * SubpixTrimScale;
    subpixBlend = min(SubpixCap, subpixBlend * subpixBlend);

    float edgeHorizontal =
        abs(lumaNW + lumaNE - 2.0 * lumaN) * 1.0 +
        abs(lumaW + lumaE - 2.0 * lumaCentre) * 2.0 +
                           abs(lumaSW + lumaSE - 2.0 * lumaS) * 1.0;

    float edgeVertical =
        abs(lumaNW + lumaSW - 2.0 * lumaW) * 1.0 +
        abs(lumaN + lumaS - 2.0 * lumaCentre) * 2.0 +
                         abs(lumaNE + lumaSE - 2.0 * lumaE) * 1.0;

    bool isHorizontal = edgeVertical >= edgeHorizontal;

    float luma1 = isHorizontal ? lumaN : lumaW;
    float luma2 = isHorizontal ? lumaS : lumaE;
    float gradient1 = luma1 - lumaCentre;
    float gradient2 = luma2 - lumaCentre;
    bool is1Steepest = abs(gradient1) >= abs(gradient2);
    float gradientScaled = 0.25 * max(abs(gradient1), abs(gradient2));

    float stepLength = isHorizontal ? texelSize.y : texelSize.x;
    float lumaLocalAvg = 0.0;

    if (is1Steepest)
    {
        stepLength = -stepLength;
        lumaLocalAvg = 0.5 * (luma1 + lumaCentre);
    }
    else
        lumaLocalAvg = 0.5 * (luma2 + lumaCentre);

    float2 currentUv = uv;
    if (isHorizontal)
        currentUv.y += stepLength * 0.5;
    else
        currentUv.x += stepLength * 0.5;

    float2 offset = isHorizontal ? float2(texelSize.x, 0.0) : float2(0.0, texelSize.y);
    float2 uv1 = currentUv - offset;
    float2 uv2 = currentUv + offset;

    float lumaEnd1 = SampleLuma(uv1) - lumaLocalAvg;
    float lumaEnd2 = SampleLuma(uv2) - lumaLocalAvg;
    bool reached1 = abs(lumaEnd1) >= gradientScaled;
    bool reached2 = abs(lumaEnd2) >= gradientScaled;
    bool reachedBoth = reached1 && reached2;

    if (!reached1)
        uv1 -= offset;

    if (!reached2)
        uv2 += offset;

    for (int i = 0; i < SearchSteps; i++)
    {
        if (reachedBoth)
            break;

        if (!reached1)
        {
            lumaEnd1 = SampleLuma(uv1) - lumaLocalAvg;
            reached1 = abs(lumaEnd1) >= gradientScaled;

            if (!reached1)
                uv1 -= offset;
        }
        if (!reached2)
        {
            lumaEnd2 = SampleLuma(uv2) - lumaLocalAvg;
            reached2 = abs(lumaEnd2) >= gradientScaled;

            if (!reached2)
                uv2 += offset;
        }

        reachedBoth = reached1 && reached2;
    }

    float distance1 = isHorizontal ? (uv.x - uv1.x) : (uv.y - uv1.y);
    float distance2 = isHorizontal ? (uv2.x - uv.x) : (uv2.y - uv.y);

    bool isDirection1 = distance1 < distance2;
    float distanceFinal = min(distance1, distance2);
    float edgeThickness = distance1 + distance2;

    float pixelOffset = -distanceFinal / edgeThickness + 0.5;

    bool isLumaCenterSmaller = lumaCentre < lumaLocalAvg;
    bool correctVariation = ((isDirection1 ? lumaEnd1 : lumaEnd2) < 0.0) != isLumaCenterSmaller;
    float finalOffset = correctVariation ? pixelOffset : 0.0;

    finalOffset = max(finalOffset, subpixBlend);

    float2 finalUv = uv;
    if (isHorizontal)
    {
        finalUv.y += finalOffset * stepLength;
    }
    else
    {
        finalUv.x += finalOffset * stepLength;
    }

    return float4(gFrame.SampleLevel(finalUv, 0).rgb, 1.0);
}