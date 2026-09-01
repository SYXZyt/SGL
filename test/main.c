#include <SGL/Window.h>
#include <SGL/Maths/Vec2.h>
#include <SGL/Maths/Vec3.h>
#include <SGL/Maths/Vec4.h>
#include <SGL/Maths/Mat4.h>
#include <SGL/Maths/Maths.h>
#include <SGL/Util/Logger.h>
#include <SGL/Runtime.h>
#include <SGL/Graphics/GraphicsDevice.h>
#include <SGL/Graphics/Shader.h>
#include <SGL/Graphics/VertexLayout.h>
#include <SGL/Graphics/VertexArray.h>
#include <SGL/Graphics/UniformBuffer.h>
#include <SGL/Graphics/PostProcess.h>
#include <SGL/Input/Keyboard.h>
#include <SGL/Input/Mouse.h>
#include <SGL/Graphics/Model.h>
#include <SGL/Graphics/Texture2D.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

typedef struct sgl_alignas(16) Vertex
{
    sgl_Vec3 pos;
    sgl_Vec2 uv;
} Vertex;

typedef struct sgl_alignas(16) Instance
{
    sgl_Vec3 offset;
    sgl_Vec4 colour;
    float scale;
} Instance;

typedef struct sgl_alignas(16) CameraUB
{
    sgl_Mat4 view;
    sgl_Mat4 proj;
} CameraUB;

typedef struct sgl_alignas(16) ModelVertex
{
    sgl_Vec3 pos;
    sgl_Vec3 normal;
    sgl_Vec2 uv;
    sgl_Vec2 __pad;
} ModelVertex;

typedef struct sgl_alignas(16) LightUB
{
    sgl_Vec3 direction;
    float ambient;
    sgl_Vec3 colour;
    float __pad;
} LightUB;

typedef struct sgl_alignas(16) FxaaParams
{
    sgl_Vec2 bufferSize;
    int isActive;
} FxaaParams;

static void ModelVertexCallback(const sgl_Model_VertexSource* source, void* outVertex, void* userdata)
{
    ModelVertex* vertex = (ModelVertex*)outVertex;

    if (source->position)
        vertex->pos = sgl_Vec3_New_ScalarXYZ(source->position[0], source->position[1], source->position[2]);

    if (source->normal)
        vertex->normal = sgl_Vec3_New_ScalarXYZ(source->normal[0], source->normal[1], source->normal[2]);

    if (source->texcoord)
        vertex->uv = sgl_Vec2_New_ScalarXY(source->texcoord[0], source->texcoord[1]);
}

typedef struct Particle
{
    sgl_Vec3 position;
    sgl_Vec3 velocity;
    float age;
    float lifetime;
} Particle;

#define PARTICLE_COUNT 128

static FxaaParams gFxaaParams;
static sgl_PostProcess* gFxaaPP;
static sgl_UniformBuffer* gFxaaUb;

static const sgl_Vec3 gEmitterPos = {{{ 0.f, -2.5f, 0.f }}};
static const float gGravity = 4.f;

static Particle gParticles[PARTICLE_COUNT];
static Instance gInstances[PARTICLE_COUNT];

static float RandRange(float lo, float hi) {
    return lo + ((float)rand() / (float)RAND_MAX) * (hi - lo);
}

static void RespawnParticle(Particle* p)
{
    p->position = gEmitterPos;
    p->velocity = sgl_Vec3_New_ScalarXYZ(RandRange(-2.f, 2.f), RandRange(4.f, 6.5f), RandRange(-2.f, 2.f));
    p->lifetime = RandRange(1.5f, 2.5f);
    p->age = 0.f;
}

static void UpdateParticles(sgl_VertexArray* va, float dt)
{
    const sgl_Vec3 colourStart = sgl_Vec3_New_ScalarXYZ(1.f, 0.9f, 0.3f); // yellow
    const sgl_Vec3 colourEnd = sgl_Vec3_New_ScalarXYZ(1.f, 0.2f, 0.1f);   // red

    for (int i = 0; i < PARTICLE_COUNT; ++i)
    {
        Particle* p = &gParticles[i];

        p->age += dt;
        if (p->age >= p->lifetime)
            RespawnParticle(p);

        p->velocity.y -= gGravity * dt;
        p->position = sgl_Vec3_Add_Vec3(p->position, sgl_Vec3_Mul_Scalar(p->velocity, dt));

        float t = sgl_Maths_Clamp(p->age / p->lifetime, 0.f, 1.f);
        sgl_Vec3 colour = sgl_Maths_Vec3_Lerp(colourStart, colourEnd, t);
        float alpha = 1.f - t;
        float scale = sgl_Maths_Lerp(0.6f, 0.1f, t);

        gInstances[i].offset = p->position;
        gInstances[i].colour = sgl_Vec4_New_ScalarXYZW(colour.x, colour.y, colour.z, alpha);
        gInstances[i].scale = scale;
    }

    sgl_VertexArray_SetInstances(va, (byte*)gInstances, PARTICLE_COUNT);
}

int main(int argc, char** argv)
{
    sgl_Runtime_Init();
    sgl_Logger_Init();

    sgl_EngineConfig cfg = sgl_EngineConfig_Default;
    cfg.enableImGui = false;
    cfg.backend = sgl_Backend_DIRECTX11;

    sgl_Window* window = sgl_Window_Create(cfg);
    sgl_GraphicsDevice* gpu = sgl_GraphicsDevice_Create(window);

    sgl_Keyboard* kb = sgl_Keyboard_New(window);

    sgl_Mouse* mouse = sgl_Mouse_New(window);
    sgl_Mouse_SetRelativeMode(mouse, true);

    gFxaaParams.bufferSize = sgl_Vec2i_to_sgl_Vec2(window->screenSize);
    gFxaaParams.isActive = true;

    gFxaaPP = sgl_PostProcess_Create(gpu, 1);
    gFxaaUb = sgl_UniformBuffer_Create(gpu, sizeof(FxaaParams));
    sgl_PostProcess_AddUniformBuffer(gFxaaPP, gFxaaUb, 0);
    sgl_UniformBuffer_Upload(gFxaaUb, &gFxaaParams);
    sgl_Shader_Load_Slang_File(sgl_PostProcess_GetShader(gFxaaPP), "PostProcess.slang", "vertexMain", "fragmentMain");

    sgl_GraphicsDevice_AddEffect(gpu, gFxaaPP);

    sgl_VertexLayout* layout = sgl_VertexLayout_New(gpu);
    {
        sgl_VertexElement pos =
        {
            .semantic = sgl_POSITION,
            .offset = offsetof(Vertex, pos),
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

        sgl_VertexElement instOffset =
        {
            .semantic = sgl_POSITION,
            .offset = offsetof(Instance, offset),
            .type = sgl_VertexElementType_VEC3,
            .perInstance = true,
        };

        sgl_VertexElement instColour =
        {
            .semantic = sgl_COLOUR,
            .offset = offsetof(Instance, colour),
            .type = sgl_VertexElementType_VEC4,
            .perInstance = true,
        };

        sgl_VertexElement instScale =
        {
            .semantic = sgl_TEXCOORD,
            .offset = offsetof(Instance, scale),
            .type = sgl_VertexElementType_FLOAT,
            .perInstance = true,
        };

        sgl_VertexLayout_Add(layout, pos);
        sgl_VertexLayout_Add(layout, uv);
        sgl_VertexLayout_Add(layout, instOffset);
        sgl_VertexLayout_Add(layout, instColour);  
        sgl_VertexLayout_Add(layout, instScale);
    }

    sgl_Shader* shader = sgl_Shader_Create(gpu, layout);
    sgl_Shader_Load_Slang_File(shader, "Basic.slang", "vertexMain", "fragmentMain");

    sgl_VertexArray* quad = sgl_VertexArray_Create(gpu, sizeof(Vertex), layout, sizeof(Instance));

    Vertex tl = { .pos = {{{-0.5f,  0.5f, 0.f}}}, .uv = {{{0.f, 0.f}}} };
    Vertex tr = { .pos = {{{ 0.5f,  0.5f, 0.f}}}, .uv = {{{1.f, 0.f}}} };
    Vertex bl = { .pos = {{{-0.5f, -0.5f, 0.f}}}, .uv = {{{0.f, 1.f}}} };
    Vertex br = { .pos = {{{ 0.5f, -0.5f, 0.f}}}, .uv = {{{1.f, 1.f}}} };

    sgl_VertexArray_Quad q = { .tl = &tr, .tr = &tl, .bl = &br, .br = &bl };
    sgl_VertexArray_AddQuad(quad, q);

    srand((unsigned int)time(NULL));

    for (int i = 0; i < PARTICLE_COUNT; ++i)
    {
        Particle* p = &gParticles[i];
        RespawnParticle(p);

        float t = RandRange(0.f, p->lifetime);

        p->velocity.y -= gGravity * t;
        p->position = sgl_Vec3_Add_Vec3(p->position, sgl_Vec3_Mul_Scalar(p->velocity, t));
        p->age = t;
    }

    sgl_VertexLayout* modelLayout = sgl_VertexLayout_New(gpu);
    {
        sgl_VertexElement pos = { .semantic = sgl_POSITION, .offset = offsetof(ModelVertex, pos), .type = sgl_VertexElementType_VEC3, .perInstance = false };
        sgl_VertexElement normal = { .semantic = sgl_NORMAL, .offset = offsetof(ModelVertex, normal), .type = sgl_VertexElementType_VEC3, .perInstance = false };
        sgl_VertexElement uv = { .semantic = sgl_TEXCOORD, .offset = offsetof(ModelVertex, uv), .type = sgl_VertexElementType_VEC2, .perInstance = false };

        sgl_VertexLayout_Add(modelLayout, pos);
        sgl_VertexLayout_Add(modelLayout, normal);
        sgl_VertexLayout_Add(modelLayout, uv);
    }

    sgl_Shader* modelShader = sgl_Shader_Create(gpu, modelLayout);
    sgl_Shader_Load_Slang_File(modelShader, "Object.slang", "vertexMain", "fragmentMain");

    sgl_Model_SetUnitScale(1.5f);
    sgl_VertexArray* suzanne = sgl_Model_Load(gpu, "suzanne.obj", sizeof(ModelVertex), modelLayout, ModelVertexCallback, NULL);

    sgl_Texture* texture = sgl_Texture2D_New_File(gpu, "stone.png", false);
    sgl_Texture* fireTexture = sgl_Texture2D_New_File(gpu, "fire.png", false);

    LightUB lightData;
    lightData.direction = sgl_Maths_Vec3_Normalise(sgl_Vec3_New_ScalarXYZ(-0.4f, 1.0f, 0.3f));
    lightData.ambient = 0.15f;
    lightData.colour = sgl_Vec3_One;

    sgl_UniformBuffer* lightUB = sgl_UniformBuffer_Create(gpu, sizeof(LightUB));
    sgl_UniformBuffer_Upload(lightUB, &lightData);

    CameraUB camData;
    camData.proj = sgl_Maths_Mat4_Perspective(sgl_Maths_Rad(60.f), (float)window->screenSize.width / window->screenSize.height, 0.1f, 100.f);

    sgl_UniformBuffer* cameraUB = sgl_UniformBuffer_Create(gpu, sizeof(CameraUB));

    sgl_Vec3 position = sgl_Vec3_New_ScalarXYZ(-7.f, 0.f, 0.f);
    float yaw = sgl_Maths_Rad(90.f);
    float pitch = 0.f;
    float moveSpeed = 3.f;

    const float mouseSensitivity = 0.0025f;
    const float pitchLimit = sgl_Maths_Rad(89.f);
    bool useMouse = true;

    typedef struct timespec timespec;
    timespec lastFrameTime;
    int _ = timespec_get(&lastFrameTime, TIME_UTC);

    while (!window->wantsClose)
    {
        timespec nowTime;
        _ = timespec_get(&nowTime, TIME_UTC);
        float dt = (float)((nowTime.tv_sec - lastFrameTime.tv_sec) + (nowTime.tv_nsec - lastFrameTime.tv_nsec) / 1000000000.0);
        lastFrameTime = nowTime;
        dt = sgl_Maths_Clamp(dt, 0.f, 0.1f);

        UpdateParticles(quad, dt);

        if (useMouse)
        {
            sgl_Vec2 mouseDelta = sgl_Mouse_GetDelta(mouse);
            yaw -= mouseDelta.x * mouseSensitivity;
            pitch -= mouseDelta.y * mouseSensitivity;
            pitch = sgl_Maths_Clamp(pitch, -pitchLimit, pitchLimit);

            sgl_Vec2 scroll = sgl_Mouse_GetScroll(mouse);
            moveSpeed = sgl_Maths_Clamp(moveSpeed + scroll.y * 0.25f, 0.5f, 20.f);
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

        if (sgl_Keyboard_IsKeyPressed(kb, sgl_Key_F1))
        {
            gFxaaParams.isActive = !gFxaaParams.isActive;
            sgl_UniformBuffer_Upload(gFxaaUb, &gFxaaParams);
        }

        if (sgl_Maths_Vec3_Length2(movement) > 0.f)
            movement = sgl_Maths_Vec3_Normalise(movement);

        position = sgl_Vec3_Add_Vec3(position, sgl_Vec3_Mul_Scalar(movement, moveSpeed * dt));

        camData.view = sgl_Maths_Mat4_LookAt(position, sgl_Vec3_Add_Vec3(position, forward), sgl_Vec3_Up);
        sgl_UniformBuffer_Upload(cameraUB, &camData);

        sgl_Window_PollEvents(window);
        sgl_Mouse_Update(mouse);
        sgl_Keyboard_Update(kb);

        sgl_UniformBuffer* modelBuffers[] = { cameraUB, lightUB };

        sgl_GraphicsDevice_BeginFrame(gpu);
        sgl_GraphicsDevice_Draw(gpu, suzanne, modelShader, &texture, 1, modelBuffers, 2);

        sgl_GraphicsDevice_SetDepthWriteEnabled(gpu, false);
        sgl_GraphicsDevice_DrawInstanced(gpu, quad, shader, &fireTexture, 1, &cameraUB, 1, PARTICLE_COUNT);
        sgl_GraphicsDevice_SetDepthWriteEnabled(gpu, true);

        sgl_GraphicsDevice_EndFrame(gpu);
        sgl_GraphicsDevice_SwapBuffer(gpu);
    }

    sgl_UniformBuffer_Destroy(gFxaaUb);
    sgl_PostProcess_Destroy(gFxaaPP);
    sgl_UniformBuffer_Destroy(cameraUB);
    sgl_UniformBuffer_Destroy(lightUB);
    sgl_Texture_Destroy(texture);
    sgl_Texture_Destroy(fireTexture);
    sgl_Shader_Destroy(shader);
    sgl_Shader_Destroy(modelShader);
    sgl_VertexArray_Destroy(quad);
    sgl_VertexArray_Destroy(suzanne);
    sgl_VertexLayout_Destroy(layout);
    sgl_VertexLayout_Destroy(modelLayout);
    sgl_Keyboard_Destroy(kb);
    sgl_Mouse_Destroy(mouse);
    sgl_GraphicsDevice_Destroy(gpu);
    sgl_Window_Destroy(window);

    sgl_Logger_Shutdown();
    sgl_Runtime_Shutdown();

    return 0;
}
