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

typedef struct sgl_alignas(16) Vertex
{
    sgl_Vec3 pos;
    sgl_Vec3 col;
} Vertex;

typedef struct sgl_alignas(16) UB
{
    sgl_Mat4 view;
    sgl_Mat4 proj;
} UB;

const char* VertexShaderSourceGL =
"#version 460 core\n"
"layout(location=0) in vec3 aPos;\n"
"layout(location=1) in vec3 aCol;\n"
"\n"
"out vec3 oCol;\n"
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
"    oCol = aCol;\n"
"}\n";

const char* FragmentShaderSourceGL =
"#version 460 core\n"
"in vec3 oCol;\n"
"out vec4 FragCol;\n"
"\n"
"void main()\n"
"{\n"
"    FragCol = vec4(oCol, 1.0);\n"
"}\n";

const char* VertexShaderSourceDX =
"struct VSInput\n"
"{\n"
"    float3 pos : POSITION;\n"
"    float3 col : COLOR;\n"
"};\n"
"\n"
"struct VSOutput\n"
"{\n"
"    float4 pos : SV_POSITION;\n"
"    float3 col : COLOR;\n"
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
"    output.col = input.col;\n"
"    return output;\n"
"}\n";

const char* PixelShaderSourceDX =
"struct PSInput\n"
"{\n"
"    float4 pos : SV_POSITION;\n"
"    float3 col : COLOR;\n"
"};\n"
"\n"
"float4 main(PSInput input) : SV_TARGET\n"
"{\n"
"    return float4(input.col, 1.0f);\n"
"}\n";

int main(int argc, char** argv)
{
    sgl_Runtime_Init();
    sgl_Memory_StackTrace(true);
    sgl_Logger_Init();

    sgl_EngineConfig cfg = sgl_EngineConfig_Default;
    cfg.backend = sgl_Backend_DIRECTX11;

    sgl_Window* window = sgl_Window_Create(cfg);
    sgl_GraphicsDevice* gpu = sgl_GraphicsDevice_Create(window);

    sgl_GraphicsDevice_ImGui_Init(gpu);

    sgl_Keyboard* kb = sgl_Keyboard_New();

    sgl_VertexLayout* layout = sgl_VertexLayout_New(gpu);

    {
        sgl_VertexElement pos =
        {
            .semantic = sgl_POSITION,
            .offset = offsetof(Vertex, pos),
            .type = sgl_VertexElementType_VEC3,
            .perInstance = false,
        };

        sgl_VertexElement col =
        {
            .semantic = sgl_COLOUR,
            .offset = offsetof(Vertex, col),
            .type = sgl_VertexElementType_VEC3,
            .perInstance = false,
        };

        sgl_VertexLayout_Add(layout, pos);
        sgl_VertexLayout_Add(layout, col);
    }

    sgl_VertexArray* va = sgl_VertexArray_Create(gpu, sizeof(Vertex), layout);

    const float scale = 0.5f;
    const float x = 64.f * scale;
    const float y = 64.f * scale;

    Vertex tl = { .pos = sgl_Vec3_New_ScalarXYZ(-x,  y, 0), .col = sgl_Colour_ToVec3(sgl_Col_Red) };
    Vertex tr = { .pos = sgl_Vec3_New_ScalarXYZ(x,  y, 0), .col = sgl_Colour_ToVec3(sgl_Col_Green) };
    Vertex bl = { .pos = sgl_Vec3_New_ScalarXYZ(-x, -y, 0), .col = sgl_Colour_ToVec3(sgl_Col_Blue) };
    Vertex br = { .pos = sgl_Vec3_New_ScalarXYZ(x, -y, 0), .col = sgl_Colour_ToVec3(sgl_Col_Yellow) };

    sgl_VertexArray_Quad q = { .tl = &tl, .tr = &tr, .bl = &bl, .br = &br };
    sgl_VertexArray_AddQuad(va, q);

    sgl_Shader* shader;

    if (cfg.backend == sgl_Backend_DIRECTX11)
        shader = sgl_Shader_Create_Source(gpu, VertexShaderSourceDX, PixelShaderSourceDX, layout);
    else
        shader = sgl_Shader_Create_Source(gpu, VertexShaderSourceGL, FragmentShaderSourceGL, layout);

    UB ubData;

    ubData.view = sgl_Maths_Mat4_View(sgl_Vec2_One, 0.0f);
    ubData.proj = sgl_Maths_Mat4_OrthographicGL(window->screenSize, 1.0f);

    sgl_UniformBuffer* ub = sgl_UniformBuffer_Create(gpu, sizeof(UB));
    sgl_UniformBuffer_Upload(ub, &ubData);

    sgl_Vec2 position = sgl_Vec2_Zero;
    while (!window->wantsClose)
    {
        {
            sgl_Vec2 movement = sgl_Vec2_Zero;

            if (sgl_Keyboard_IsKeyDown(kb, sgl_Key_A))
                movement.x -= .01f;
            if (sgl_Keyboard_IsKeyDown(kb, sgl_Key_D))
                movement.x += .01f;

            if (sgl_Keyboard_IsKeyDown(kb, sgl_Key_W))
                movement.y -= .01f;
            if (sgl_Keyboard_IsKeyDown(kb, sgl_Key_S))
                movement.y += .01f;

            if (sgl_Maths_Vec2_Length2(movement) > 0.f)
            {
                movement = sgl_Maths_Vec2_Normalise(movement);
                position = sgl_Vec2_Add_Vec2(position, movement);

                ubData.view = sgl_Maths_Mat4_View(position, 0.0f);
                sgl_UniformBuffer_Upload(ub, &ubData);
            }
        }

        sgl_Window_PollEvents(window);
        sgl_Keyboard_Update(kb);

        sgl_GraphicsDevice_Clear(gpu);

        sgl_GraphicsDevice_Draw(gpu, va, shader, &ub, 1);

        sgl_GraphicsDevice_ImGui_NewFrame(gpu);

        sgl_TextUnformatted("Hello, World!");

        sgl_GraphicsDevice_ImGui_RenderDrawData(gpu);

        sgl_GraphicsDevice_Present(gpu);
    }

    sgl_Keyboard_Destroy(kb);
    sgl_GraphicsDevice_ImGui_Shutdown(gpu);
    sgl_UniformBuffer_Destroy(ub);
    sgl_Shader_Destroy(shader);
    sgl_VertexArray_Destroy(va);
    sgl_VertexLayout_Destroy(layout);
    sgl_GraphicsDevice_Destroy(gpu);
    sgl_Window_Destroy(window);

    sgl_Logger_Shutdown();
    sgl_Runtime_Shutdown();

    return 0;
}