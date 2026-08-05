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
#include <SGL/Graphics/Model.h>
#include <threads.h>

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

typedef struct sgl_alignas(16) PostProcessEffectUniforms
{
    float time;
    sgl_Vec3 __pad;
} PostProcessEffectUniforms;

static sgl_Texture* gTexture = NULL;
static sgl_VertexArray* gSuzanne = NULL;
static sgl_Shader* gShader = NULL;

static volatile bool gHasLoadingBeenDone = false;

static void LoadingThread()
{
    gHasLoadingBeenDone = true;
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

    sgl_Vec3 qtl = sgl_Vec3_New_ScalarXYZ(-x, y, 0);
    sgl_Vec3 qtr = sgl_Vec3_New_ScalarXYZ(x, y, 0);
    sgl_Vec3 qbl = sgl_Vec3_New_ScalarXYZ(-x, -y, 0);
    sgl_Vec3 qbr = sgl_Vec3_New_ScalarXYZ(x, -y, 0);

    sgl_Vec3 quadNormal = sgl_Maths_Vec3_Normalise(sgl_Maths_Vec3_Cross(sgl_Vec3_Sub_Vec3(qtr, qtl), sgl_Vec3_Sub_Vec3(qbr, qtl)));

    Vertex tl = { .pos = qtl, .normal = quadNormal, .uv = sgl_Vec2_Up };
    Vertex tr = { .pos = qtr, .normal = quadNormal, .uv = sgl_Vec2_One };
    Vertex bl = { .pos = qbl, .normal = quadNormal, .uv = sgl_Vec2_Zero };
    Vertex br = { .pos = qbr, .normal = quadNormal, .uv = sgl_Vec2_Right };

    sgl_VertexArray_AddVertex(va, &tl);
    sgl_VertexArray_AddVertex(va, &tr);
    sgl_VertexArray_AddVertex(va, &bl);
    sgl_VertexArray_AddVertex(va, &br);

    sgl_VertexArray_AddTriIndices(va, 0, 1, 3);
    sgl_VertexArray_AddTriIndices(va, 0, 3, 2);

    sgl_VertexArray* suzanne = sgl_Model_Load(gpu, "suzanne.obj", sizeof(Vertex), layout);

    sgl_Shader* shader;

    sgl_PostProcess* postProcessEffect = sgl_PostProcess_Create(gpu, 1);

    PostProcessEffectUniforms ppUniforms;
    ppUniforms.time = 0;

    sgl_UniformBuffer* ubPp = sgl_UniformBuffer_Create(gpu, sizeof(PostProcessEffectUniforms));
    sgl_PostProcess_AddUniformBuffer(postProcessEffect, ubPp, 0);

    shader = sgl_Shader_Create(gpu, layout);
    sgl_Shader_Load_Slang_File(shader, "Object.slang", "vertexMain", "fragmentMain");

    sgl_Shader_Load_Slang_File(postProcessEffect->shader, "PostProcess.slang", "vertexMain", "fragmentMain");

    sgl_GraphicsDevice_AddEffect(gpu, postProcessEffect);

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

    sgl_Vec3 position = sgl_Vec3_New_ScalarXYZ(0.f, 0.f, 150.f);
    float yaw = sgl_Maths_ATan2(100.f - position.x, 0.f - position.z); /* start facing the cube */
    float pitch = 0.f;
    float moveSpeed = 1.f;

    const float mouseSensitivity = 0.0025f;
    const float pitchLimit = sgl_Maths_Rad(89.f);
    bool useMouse = true;

    while (!window->wantsClose)
    {
        {
            ppUniforms.time += 0.1f;
            sgl_UniformBuffer_Upload(ubPp, &ppUniforms);

            if (useMouse)
            {
                sgl_Vec2 mouseDelta = sgl_Mouse_GetDelta(mouse);
                yaw -= mouseDelta.x * mouseSensitivity;
                pitch -= mouseDelta.y * mouseSensitivity;
                pitch = sgl_Maths_Clamp(pitch, -pitchLimit, pitchLimit);

                sgl_Vec2 scroll = sgl_Mouse_GetScroll(mouse);
                moveSpeed = sgl_Maths_Clamp(moveSpeed + scroll.y * 0.25f, 0.1f, 10.f);
            }

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

            if (sgl_Keyboard_IsKeyPressed(kb, sgl_Key_ESCAPE))
            {
                useMouse = !useMouse;
                sgl_Mouse_SetRelativeMode(mouse, useMouse);
            }

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
        sgl_GraphicsDevice_Draw(gpu, suzanne, shader, &texture, 1, frameBuffers, 2);
        sgl_GraphicsDevice_Draw(gpu, va, shader, &texture, 1, frameBuffers, 2);

        if (sgl_InputFloat("Time", &ppUniforms.time, 1, 1, 0))
            sgl_UniformBuffer_Upload(ubPp, &ppUniforms);

        {
            bool depthTestEnabled = sgl_GraphicsDevice_GetDepthTestEnabled(gpu);
            if (sgl_Checkbox("Depth Test", &depthTestEnabled))
                sgl_GraphicsDevice_SetDepthTestEnabled(gpu, depthTestEnabled);
        }

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
    sgl_VertexArray_Destroy(suzanne);
    sgl_VertexArray_Destroy(va);
    sgl_VertexLayout_Destroy(layout);
    sgl_GraphicsDevice_Destroy(gpu);
    sgl_Window_Destroy(window);

    sgl_Logger_Shutdown();
    sgl_Runtime_Shutdown();

    return 0;
}