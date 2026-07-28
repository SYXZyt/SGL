#include <SGL/Window.h>
#include <SGL/Maths/Vec2.h>
#include <SGL/Maths/Mat4.h>
#include <SGL/Maths/Maths.h>
#include <SGL/Util/Logger.h>
#include <SGL/Util/Memory.h>
#include <SGL/Runtime.h>
#include <SGL/Graphics/GraphicsDevice.h>
#include <SGL/Graphics/Shader.h>
#include <SGL/Graphics/VertexLayout.h>
#include <SGL/Graphics/VertexArray.h>
#include <SGL/Graphics/UniformBuffer.h>
#include <SGL/ImGui/ImGui.h>
#include <SGL/Input/Keyboard.h>
#include <SGL/Input/Mouse.h>
#include <SGL/Graphics/PostProcess.h>
#include <SGL/Graphics/Texture2D.h>
#include <SGL/Graphics/Texture2DArray.h>
#include <stdio.h>

typedef struct sgl_alignas(16) Vertex
{
    sgl_Vec3 pos;
    sgl_Vec3 normal;
    sgl_Vec2 uv;
    sgl_Vec2 __pad;
} Vertex;

typedef struct sgl_alignas(16) UB
{
    sgl_Mat4 view;
    sgl_Mat4 proj;
} UB;

/* Directional light. Vertices are already in world space (no model matrix
 * in this demo), so normals need no extra transform - straight through. */
typedef struct sgl_alignas(16) LightUB
{
    sgl_Vec3 direction; /* world-space direction FROM a surface TOWARD the light */
    float ambient;
    sgl_Vec3 colour;
    float __pad;
} LightUB;

const char* VertexShaderSourceGL =
"#version 460 core\n"
"layout(location=0) in vec3 aPos;\n"
"layout(location=1) in vec3 aNormal;\n"
"layout(location=2) in vec2 aUV;\n"
"\n"
"out vec2 oUV;\n"
"out vec3 oNormal;\n"
"\n"
"layout(std140, binding=0) uniform CameraBuffer\n"
"{\n"
"    mat4 view;\n"
"    mat4 proj;\n"
"};\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = proj * view * vec4(aPos, 1.0);\n"
"    oUV = aUV;\n"
"    oNormal = aNormal;\n"
"}\n";

const char* FragmentShaderSourceGL =
"#version 460 core\n"
"in vec2 oUV;\n"
"in vec3 oNormal;\n"
"out vec4 FragCol;\n"
"\n"
"layout(binding = 0) uniform sampler2DArray tex;\n"
"\n"
"layout(std140, binding=1) uniform LightBuffer\n"
"{\n"
"    vec3 lightDir;\n"
"    float ambient;\n"
"    vec3 lightColour;\n"
"};\n"
"\n"
"void main()\n"
"{\n"
"    vec3 n = normalize(oNormal);\n"
"    float ndotl = max(dot(n, normalize(lightDir)), 0.0);\n"
"    vec3 lighting = lightColour * (ambient + (1.0 - ambient) * ndotl);\n"
"\n"
"    vec4 texColour = texture(tex, vec3(oUV, 3));\n"
"    FragCol = vec4(texColour.rgb * lighting, texColour.a);\n"
"}\n";

const char* VertexShaderSourceDX =
"struct VSInput\n"
"{\n"
"    float3 pos : POSITION;\n"
"    float3 normal : NORMAL;\n"
"    float2 uv : TEXCOORD;\n"
"};\n"
"\n"
"struct VSOutput\n"
"{\n"
"    float4 pos : SV_POSITION;\n"
"    float3 normal : NORMAL;\n"
"    float2 uv : TEXCOORD;\n"
"};\n"
"\n"
"cbuffer CameraMatrices : register(b0)\n"
"{\n"
"    float4x4 View;\n"
"    float4x4 Proj;\n"
"};\n"
"\n"
"VSOutput main(VSInput input)\n"
"{\n"
"    VSOutput output;\n"
"    output.pos = mul(Proj, mul(View, float4(input.pos, 1.0f)));\n"
"    output.normal = input.normal;\n"
"    output.uv = input.uv;\n"
"    return output;\n"
"}\n";

const char* PixelShaderSourceDX =
"struct PSInput\n"
"{\n"
"    float4 pos : SV_POSITION;\n"
"    float3 normal : NORMAL;\n"
"    float2 uv : TEXCOORD;\n"
"};\n"
"\n"
"Texture2DArray tex : register(t0);\n"
"SamplerState texSampler : register(s0);\n"
"\n"
"cbuffer LightBuffer : register(b1)\n"
"{\n"
"    float3 lightDir;\n"
"    float ambient;\n"
"    float3 lightColour;\n"
"    float _pad;\n"
"};\n"
"\n"
"float4 main(PSInput input) : SV_TARGET\n"
"{\n"
"   float3 n = normalize(input.normal);\n"
"   float ndotl = max(dot(n, normalize(lightDir)), 0.0f);\n"
"   float3 lighting = lightColour * (ambient + (1.0f - ambient) * ndotl);\n"
"\n"
"   float4 texColour = tex.Sample(texSampler, float3(input.uv, 3));\n"
"   return float4(texColour.rgb * lighting, texColour.a);\n"
"}\n";

const char* PostProcessEffectVertexGL =
"#version 460 core\n"
"layout(location=0) in vec3 aPos;\n"
"layout(location=1) in vec2 aUV;\n"
"\n"
"out vec2 oUV;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos, 1.0);\n"
"    oUV = aUV;\n"
"}\n";

const char* PostProcessEffectFragmentGL =
"#version 460 core\n"
"in vec2 oUV;\n"
"out vec4 FragCol;\n"
"\n"
"layout(std140, binding=0) uniform TimeBuffer\n"
"{\n"
"    float time;\n"
"    vec3 _pad;\n"
"};\n"
"layout(binding = 0) uniform sampler2D frametexture;"
"void main()\n"
"{\n"
"   FragCol = texture(frametexture, oUV);\n"
"   vec3 rgb = FragCol.rgb;\n"
"   vec3 inv_rgb = 1 - FragCol.rgb;\n"
"   FragCol.rgb = mix(rgb, inv_rgb, sin(time * 0.2));"
"}\n";

const char* PostProcessEffectVertexHLSL =
"struct VSInput\n"
"{\n"
"    float3 Pos : POSITION;\n"
"    float2 UV : TEXCOORD0;\n"
"};\n"
"\n"
"struct PSInput\n"
"{\n"
"    float4 Pos : SV_POSITION;\n"
"    float2 UV : TEXCOORD0;\n"
"};\n"
"\n"
"PSInput main(VSInput input)\n"
"{\n"
"    PSInput output;\n"
"    output.Pos = float4(input.Pos, 1.0f);\n"
"    output.UV = input.UV;\n"
"    return output;\n"
"}\n";

const char* PostProcessEffectPixelHLSL =
"Texture2D FrameTexture : register(t0);\n"
"SamplerState FrameSampler : register(s0);\n"
"\n"
"struct PSInput\n"
"{\n"
"    float4 Pos : SV_POSITION;\n"
"    float2 UV : TEXCOORD0;\n"
"};\n"
"\n"
"cbuffer TimeBuffer : register(b0)\n"
"{\n"
"   float time;\n"
"   float3 __pad;\n"
"};\n"
"float4 main(PSInput input) : SV_TARGET\n"
"{\n"
"   float4 colour = FrameTexture.Sample(FrameSampler, input.UV);\n"
"   float3 rgb = colour.rgb;\n"
"   float3 inv_rgb = 1 - colour.rgb;\n"
"   float3 finalColour = lerp(rgb, inv_rgb, sin(time * 0.2));\n"
"   return float4(finalColour, 1);\n"
"}\n";

typedef struct sgl_alignas(16) PostProcessEffectUniforms
{
    float time;
    sgl_Vec3 __pad;
} PostProcessEffectUniforms;

/* ---------------------------------------------------------------------
 * Cube helpers
 *
 * 4 unique vertices per face, indexed as two triangles, 6 faces.
 * Faces can't share vertices with each other even though their corner
 * positions coincide in 3D space: each face needs its own UV at that
 * corner (a cube's per-face 0-1 texture tiling requires it), so this is
 * 24 unique (pos, uv) vertices + 36 indices, not 8 vertices + 36 indices.
 * Position convention follows the original quad (tl/tr have +Y, bl/br
 * have -Y) so it matches how the rest of this file lays out geometry.
 * --------------------------------------------------------------------- */

static void AddCubeFace(sgl_VertexArray* va, sgl_Vec3 tl, sgl_Vec3 tr, sgl_Vec3 bl, sgl_Vec3 br)
{
    /* Flat-shaded: every vertex on this face shares the same outward-facing
     * normal. Matches the winding fixed earlier: outward = Cross(tl-tr, bl-tr). */
    sgl_Vec3 normal = sgl_Maths_Vec3_Normalise(sgl_Maths_Vec3_Cross(sgl_Vec3_Sub_Vec3(tl, tr), sgl_Vec3_Sub_Vec3(bl, tr)));

    Vertex vtl = { .pos = tl, .normal = normal, .uv = sgl_Vec2_Up };
    Vertex vtr = { .pos = tr, .normal = normal, .uv = sgl_Vec2_One };
    Vertex vbl = { .pos = bl, .normal = normal, .uv = sgl_Vec2_Zero };
    Vertex vbr = { .pos = br, .normal = normal, .uv = sgl_Vec2_Right };

    uint32 base = va->vertexCount;

    sgl_VertexArray_AddVertex(va, &vtl);
    sgl_VertexArray_AddVertex(va, &vtr);
    sgl_VertexArray_AddVertex(va, &vbl);
    sgl_VertexArray_AddVertex(va, &vbr);

    /* Same winding as sgl_Triangulate() would've produced via AddQuad with
     * the diagonal-swapped {tl=vtr, tr=vtl, bl=vbr, br=vbl} that fixed the
     * inward-facing cube: t0=(vtr,vtl,vbl), t1=(vtr,vbl,vbr). */
    sgl_VertexArray_AddTriIndices(va, base + 1, base + 0, base + 2);
    sgl_VertexArray_AddTriIndices(va, base + 1, base + 2, base + 3);
}

static void AddCube(sgl_VertexArray* va, sgl_Vec3 center, float halfSize)
{
    const float hs = halfSize;

    sgl_Vec3 ftl = sgl_Vec3_New_ScalarXYZ(center.x - hs, center.y + hs, center.z + hs);
    sgl_Vec3 ftr = sgl_Vec3_New_ScalarXYZ(center.x + hs, center.y + hs, center.z + hs);
    sgl_Vec3 fbl = sgl_Vec3_New_ScalarXYZ(center.x - hs, center.y - hs, center.z + hs);
    sgl_Vec3 fbr = sgl_Vec3_New_ScalarXYZ(center.x + hs, center.y - hs, center.z + hs);

    sgl_Vec3 btl = sgl_Vec3_New_ScalarXYZ(center.x - hs, center.y + hs, center.z - hs);
    sgl_Vec3 btr = sgl_Vec3_New_ScalarXYZ(center.x + hs, center.y + hs, center.z - hs);
    sgl_Vec3 bbl = sgl_Vec3_New_ScalarXYZ(center.x - hs, center.y - hs, center.z - hs);
    sgl_Vec3 bbr = sgl_Vec3_New_ScalarXYZ(center.x + hs, center.y - hs, center.z - hs);

    AddCubeFace(va, ftl, ftr, fbl, fbr); /* front  (+Z) */
    AddCubeFace(va, btr, btl, bbr, bbl); /* back   (-Z) */
    AddCubeFace(va, btl, ftl, bbl, fbl); /* left   (-X) */
    AddCubeFace(va, ftr, btr, fbr, bbr); /* right  (+X) */
    AddCubeFace(va, btl, btr, ftl, ftr); /* top    (+Y) */
    AddCubeFace(va, fbl, fbr, bbl, bbr); /* bottom (-Y) */
}

int main(int argc, char** argv)
{
    sgl_Runtime_Init();
    sgl_Memory_StackTrace(true);
    sgl_Logger_Init();

    sgl_EngineConfig cfg = sgl_EngineConfig_Default;
    cfg.enableImGui = true;

    cfg.backend = sgl_Backend_DIRECTX11;

    sgl_Window* window = sgl_Window_Create(cfg);
    sgl_GraphicsDevice* gpu = sgl_GraphicsDevice_Create(window);

    sgl_GraphicsDevice_ImGui_Init(gpu);

    sgl_Keyboard* kb = sgl_Keyboard_New(window);

    sgl_Mouse* mouse = sgl_Mouse_New(window);
    sgl_Mouse_SetRelativeMode(mouse, true);

    sgl_VertexLayout* layout = sgl_VertexLayout_New(gpu);

    {
        sgl_VertexElement pos =
        {
            .semantic = sgl_POSITION,
            .offset = offsetof(Vertex, pos),
            .type = sgl_VertexElementType_VEC3,
            .perInstance = false,
        };

        sgl_VertexElement normal =
        {
            .semantic = sgl_NORMAL,
            .offset = offsetof(Vertex, normal),
            .type = sgl_VertexElementType_VEC3,
            .perInstance = false,
        };

        sgl_VertexElement uv =
        {
            .semantic = sgl_TEXCOORD,
            .offset = offsetof(Vertex, uv),
            .type = sgl_VertexElementType_VEC2,
            .perInstance = false,
        };

        sgl_VertexLayout_Add(layout, pos);
        sgl_VertexLayout_Add(layout, normal);
        sgl_VertexLayout_Add(layout, uv);
    }


    sgl_VertexArray* va = sgl_VertexArray_Create(gpu, sizeof(Vertex), layout);

    const float scale = 0.5f;
    const float x = 64.f * scale;
    const float y = 64.f * scale;

    sgl_Vec3 qtl = sgl_Vec3_New_ScalarXYZ(-x,  y, 0);
    sgl_Vec3 qtr = sgl_Vec3_New_ScalarXYZ(x,  y, 0);
    sgl_Vec3 qbl = sgl_Vec3_New_ScalarXYZ(-x, -y, 0);
    sgl_Vec3 qbr = sgl_Vec3_New_ScalarXYZ(x, -y, 0);

    /* Matches this quad's (unswapped) winding: t0=(tl,tr,br). */
    sgl_Vec3 quadNormal = sgl_Maths_Vec3_Normalise(sgl_Maths_Vec3_Cross(sgl_Vec3_Sub_Vec3(qtr, qtl), sgl_Vec3_Sub_Vec3(qbr, qtl)));

    Vertex tl = { .pos = qtl, .normal = quadNormal, .uv = sgl_Vec2_Up };
    Vertex tr = { .pos = qtr, .normal = quadNormal, .uv = sgl_Vec2_One };
    Vertex bl = { .pos = qbl, .normal = quadNormal, .uv = sgl_Vec2_Zero };
    Vertex br = { .pos = qbr, .normal = quadNormal, .uv = sgl_Vec2_Right };

    sgl_VertexArray_AddVertex(va, &tl);
    sgl_VertexArray_AddVertex(va, &tr);
    sgl_VertexArray_AddVertex(va, &bl);
    sgl_VertexArray_AddVertex(va, &br);

    /* Same winding sgl_Triangulate() would've produced via AddQuad(tl,tr,bl,br):
     * t0=(tl,tr,br), t1=(tl,br,bl). */
    sgl_VertexArray_AddTriIndices(va, 0, 1, 3);
    sgl_VertexArray_AddTriIndices(va, 0, 3, 2);

    /* --- New: a 3D cube, built in its own vertex array so it can sit
     * alongside the ground quad. Placed off to the side so the two
     * don't overlap. --- */
    sgl_VertexArray* cubeVA = sgl_VertexArray_Create(gpu, sizeof(Vertex), layout);
    AddCube(cubeVA, sgl_Vec3_New_ScalarXYZ(100.f, 0.f, 0.f), 32.f);

    sgl_Shader* shader;

    sgl_PostProcess* postProcessEffect = sgl_PostProcess_Create(gpu, 1);

    PostProcessEffectUniforms ppUniforms;
    ppUniforms.time = 0;

    sgl_UniformBuffer* ubPp = sgl_UniformBuffer_Create(gpu, sizeof(PostProcessEffectUniforms));
    sgl_PostProcess_AddUniformBuffer(postProcessEffect, ubPp, 0);

    if (cfg.backend == sgl_Backend_DIRECTX11)
    {
        shader = sgl_Shader_Create(gpu, layout);
        sgl_Shader_Load_Source(shader, VertexShaderSourceDX, PixelShaderSourceDX);

        sgl_Shader_Load_Source(postProcessEffect->shader, PostProcessEffectVertexHLSL, PostProcessEffectPixelHLSL);
    }
    else
    {
        shader = sgl_Shader_Create(gpu, layout);
        sgl_Shader_Load_Source(shader, VertexShaderSourceGL, FragmentShaderSourceGL);

        sgl_Shader_Load_Source(postProcessEffect->shader, PostProcessEffectVertexGL, PostProcessEffectFragmentGL);
    }

    //sgl_GraphicsDevice_AddEffect(gpu, postProcessEffect);

    UB ubData;

    ubData.view = sgl_Maths_Mat4_View(sgl_Vec3_One, 0.0f);
    ubData.proj = sgl_Maths_Mat4_OrthographicGL(window->screenSize, 1.0f);
    ubData.proj = sgl_Maths_Mat4_Perspective(45.f, (float)window->screenSize.width / window->screenSize.height, 0.1, 1000);

    sgl_UniformBuffer* ub = sgl_UniformBuffer_Create(gpu, sizeof(UB));
    sgl_UniformBuffer_Upload(ub, &ubData);

    LightUB lightData;
    lightData.direction = sgl_Maths_Vec3_Normalise(sgl_Vec3_New_ScalarXYZ(-0.4f, 1.0f, 0.3f));
    lightData.ambient = 0.15f;
    lightData.colour = sgl_Vec3_One;

    sgl_UniformBuffer* lightUB = sgl_UniformBuffer_Create(gpu, sizeof(LightUB));
    sgl_UniformBuffer_Upload(lightUB, &lightData);

    sgl_Texture* texture = sgl_Texture2DArray_New_File(gpu, "stone.png", sgl_Vec2i_New_Scalar(16));

    /* FPS camera state: position + yaw/pitch (radians). Yaw is measured so
     * that forward = (sin(yaw), ., cos(yaw)); mouse moving right should turn
     * the camera right, which - given sgl_Maths_Mat4_LookAt's actual screen
     * axes - means DECREASING yaw (verified empirically: at yaw=0, world -X
     * is what renders on the right side of the screen, not +X). */
    sgl_Vec3 position = sgl_Vec3_New_ScalarXYZ(0.f, 0.f, 150.f);
    float yaw = sgl_Maths_ATan2(100.f - position.x, 0.f - position.z); /* start facing the cube */
    float pitch = 0.f;
    float moveSpeed = 1.f;

    const float mouseSensitivity = 0.0025f;
    const float pitchLimit = sgl_Maths_Rad(89.f);

    while (!window->wantsClose)
    {
        {
            ppUniforms.time += 0.1f;
            sgl_UniformBuffer_Upload(ubPp, &ppUniforms);

            sgl_Vec2 mouseDelta = sgl_Mouse_GetDelta(mouse);
            yaw -= mouseDelta.x * mouseSensitivity;
            pitch -= mouseDelta.y * mouseSensitivity;
            pitch = sgl_Maths_Clamp(pitch, -pitchLimit, pitchLimit);

            sgl_Vec2 scroll = sgl_Mouse_GetScroll(mouse);
            moveSpeed = sgl_Maths_Clamp(moveSpeed + scroll.y * 0.25f, 0.1f, 10.f);

            /* Full 3D facing direction, used for looking; flattened (pitch-less)
             * forward used for movement so looking up/down doesn't fly you
             * into the ground/sky. */
            sgl_Vec3 forward = sgl_Vec3_New_ScalarXYZ(
                sgl_Maths_Cos(pitch) * sgl_Maths_Sin(yaw),
                sgl_Maths_Sin(pitch),
                sgl_Maths_Cos(pitch) * sgl_Maths_Cos(yaw));

            sgl_Vec3 flatForward = sgl_Vec3_New_ScalarXYZ(sgl_Maths_Sin(yaw), 0.f, sgl_Maths_Cos(yaw));
            sgl_Vec3 right = sgl_Maths_Vec3_Cross(flatForward, sgl_Vec3_Up);

            sgl_Vec3 movement = sgl_Vec3_Zero;

            if (sgl_Keyboard_IsKeyDown(kb, sgl_Key_W))
                movement = sgl_Vec3_Add_Vec3(movement, flatForward);
            if (sgl_Keyboard_IsKeyDown(kb, sgl_Key_S))
                movement = sgl_Vec3_Sub_Vec3(movement, flatForward);

            if (sgl_Keyboard_IsKeyDown(kb, sgl_Key_D))
                movement = sgl_Vec3_Add_Vec3(movement, right);
            if (sgl_Keyboard_IsKeyDown(kb, sgl_Key_A))
                movement = sgl_Vec3_Sub_Vec3(movement, right);

            if (sgl_Keyboard_IsKeyDown(kb, sgl_Key_SPACE))
                movement.y += 1.f;
            if (sgl_Keyboard_IsKeyDown(kb, sgl_Key_LCTRL))
                movement.y -= 1.f;

            if (sgl_Maths_Vec3_Length2(movement) > 0.f)
                movement = sgl_Maths_Vec3_Normalise(movement);

            position = sgl_Vec3_Add_Vec3(position, sgl_Vec3_Mul_Scalar(movement, moveSpeed));

            ubData.view = sgl_Maths_Mat4_LookAt(position, sgl_Vec3_Add_Vec3(position, forward), sgl_Vec3_Up);
            sgl_UniformBuffer_Upload(ub, &ubData);
        }

        sgl_Window_PollEvents(window);
        sgl_Mouse_Update(mouse);
        sgl_Keyboard_Update(kb);

        sgl_GraphicsDevice_ImGui_NewFrame(gpu);

        sgl_UniformBuffer* frameBuffers[] = { ub, lightUB };

        sgl_GraphicsDevice_BeginFrame(gpu);
        sgl_GraphicsDevice_Draw(gpu, va, shader, &texture, 1, frameBuffers, 2);
        sgl_GraphicsDevice_Draw(gpu, cubeVA, shader, &texture, 1, frameBuffers, 2);

        if (sgl_InputFloat("Time", &ppUniforms.time, 1, 1, 0))
            sgl_UniformBuffer_Upload(ubPp, &ppUniforms);

        sgl_GraphicsDevice_EndFrame(gpu);
        sgl_GraphicsDevice_ImGui_RenderDrawData(gpu);
        sgl_GraphicsDevice_SwapBuffer(gpu);
    }

    sgl_UniformBuffer_Destroy(ubPp);
    sgl_PostProcess_Destroy(postProcessEffect);
    sgl_Texture_Destroy(texture);
    sgl_Keyboard_Destroy(kb);
    sgl_Mouse_Destroy(mouse);
    sgl_GraphicsDevice_ImGui_Shutdown(gpu);
    sgl_UniformBuffer_Destroy(ub);
    sgl_UniformBuffer_Destroy(lightUB);
    sgl_Shader_Destroy(shader);
    sgl_VertexArray_Destroy(cubeVA);
    sgl_VertexArray_Destroy(va);
    sgl_VertexLayout_Destroy(layout);
    sgl_GraphicsDevice_Destroy(gpu);
    sgl_Window_Destroy(window);

    sgl_Logger_Shutdown();
    sgl_Runtime_Shutdown();

    return 0;
}