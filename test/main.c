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
#include <time.h>

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

typedef struct sgl_alignas(16) FxaaParams
{
    sgl_Vec2 resolution;
    int isActive;
} FxaaParams;

static sgl_Texture* gTexture = NULL;
static sgl_VertexArray* gSuzanne = NULL;
static sgl_Shader* gShader = NULL;

static volatile bool gHasLoadingBeenDone = false;

static sgl_GraphicsDevice* gGPU;
static sgl_VertexLayout* gLayout;

typedef struct timespec timespec;

static int LoadingThread(void* arg)
{
    gShader = sgl_Shader_Create(gGPU, gLayout);
    sgl_Shader_Load_Slang_File(gShader, "Object.slang", "vertexMain", "fragmentMain");

    gSuzanne = sgl_Model_Load(gGPU, "suzanne.obj", sizeof(Vertex), gLayout);
    gTexture = sgl_Texture2DArray_New_File(gGPU, "stone.png", sgl_Vec2i_New_Scalar(16));

    gHasLoadingBeenDone = true;
    return 0;
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
    gGPU = sgl_GraphicsDevice_Create(window);

    sgl_GraphicsDevice_ImGui_Init(gGPU);

    sgl_Keyboard* kb = sgl_Keyboard_New(window);

    sgl_Mouse* mouse = sgl_Mouse_New(window);
    sgl_Mouse_SetRelativeMode(mouse, true);

    sgl_VertexLayout* layout = sgl_VertexLayout_New(gGPU);
    gLayout = layout;
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

    thrd_t thread;

    timespec start;
    timespec end;

    int _ = timespec_get(&start, TIME_UTC);
    thrd_create(&thread, LoadingThread, NULL);

    // Busy wait while waiting for resource loading
    sgl_Log("Waiting for resource loading");
    while (!gHasLoadingBeenDone)
        thrd_yield();

    thrd_join(thread, NULL);

    _ = timespec_get(&end, TIME_UTC);

    double elapsed =
        (double)(end.tv_sec - start.tv_sec) +
        (double)(end.tv_nsec - start.tv_nsec) / 1000000000.0;

    char time[128];
    memset(time, '\0', 128);
    sprintf(time, "Resource loading done in %.3f seconds", elapsed);
    sgl_Log(time);

    sgl_PostProcess* postProcessEffect = sgl_PostProcess_Create(gGPU, 1);

    FxaaParams ppUniforms;
    ppUniforms.resolution = sgl_Vec2i_to_sgl_Vec2(window->screenSize);
    ppUniforms.isActive = true;

    sgl_UniformBuffer* ubPp = sgl_UniformBuffer_Create(gGPU, sizeof(FxaaParams));
    sgl_UniformBuffer_Upload(ubPp, &ppUniforms);

    sgl_PostProcess_AddUniformBuffer(postProcessEffect, ubPp, 0);

    sgl_Shader_Load_Slang_File(postProcessEffect->shader, "PostProcess.slang", "vertexMain", "fragmentMain");

    sgl_GraphicsDevice_AddEffect(gGPU, postProcessEffect);

    UB ubData;

    ubData.view = sgl_Maths_Mat4_View(sgl_Vec3_One, 0.0f);
    ubData.proj = sgl_Maths_Mat4_OrthographicGL(window->screenSize, 1.0f);
    ubData.proj = sgl_Maths_Mat4_Perspective(45.f, (float)window->screenSize.width / window->screenSize.height, 0.1f, 1000);

    sgl_UniformBuffer* ub = sgl_UniformBuffer_Create(gGPU, sizeof(UB));
    sgl_UniformBuffer_Upload(ub, &ubData);

    LightUB lightData;
    lightData.direction = sgl_Maths_Vec3_Normalise(sgl_Vec3_New_ScalarXYZ(-0.4f, 1.0f, 0.3f));
    lightData.ambient = 0.15f;
    lightData.colour = sgl_Vec3_One;

    sgl_UniformBuffer* lightUB = sgl_UniformBuffer_Create(gGPU, sizeof(LightUB));
    sgl_UniformBuffer_Upload(lightUB, &lightData);

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

        sgl_GraphicsDevice_ImGui_NewFrame(gGPU);

        sgl_UniformBuffer* frameBuffers[] = { ub, lightUB };

        sgl_GraphicsDevice_BeginFrame(gGPU);
        sgl_GraphicsDevice_Draw(gGPU, gSuzanne, gShader, &gTexture, 1, frameBuffers, 2);

        if (sgl_Checkbox_intbool("FXAA", &ppUniforms.isActive, 1, 1, 0))
            sgl_UniformBuffer_Upload(ubPp, &ppUniforms);

        {
            bool depthTestEnabled = sgl_GraphicsDevice_GetDepthTestEnabled(gGPU);
            if (sgl_Checkbox("Depth Test", &depthTestEnabled))
                sgl_GraphicsDevice_SetDepthTestEnabled(gGPU, depthTestEnabled);
        }

        sgl_GraphicsDevice_EndFrame(gGPU);
        sgl_GraphicsDevice_ImGui_RenderDrawData(gGPU);
        sgl_GraphicsDevice_SwapBuffer(gGPU);
    }

    sgl_UniformBuffer_Destroy(ubPp);
    sgl_PostProcess_Destroy(postProcessEffect);
    sgl_Texture_Destroy(gTexture);
    sgl_Keyboard_Destroy(kb);
    sgl_Mouse_Destroy(mouse);
    sgl_GraphicsDevice_ImGui_Shutdown(gGPU);
    sgl_UniformBuffer_Destroy(ub);
    sgl_UniformBuffer_Destroy(lightUB);
    sgl_Shader_Destroy(gShader);
    sgl_VertexArray_Destroy(gSuzanne);
    sgl_VertexLayout_Destroy(layout);
    sgl_GraphicsDevice_Destroy(gGPU);
    sgl_Window_Destroy(window);

    sgl_Logger_Shutdown();
    sgl_Runtime_Shutdown();

    return 0;
}