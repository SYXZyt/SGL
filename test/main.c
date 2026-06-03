#include <SGL/Window.h>
#include <SGL/Maths/Vec3.h>
#include <SGL/Util/ANSI.h>
#include <stdio.h>
#include <SGL/Util/Logger.h>
#include <SGL/Graphics/GraphicsDevice.h>
#include <SGL/Graphics/Shader.h>
#include <SGL/Graphics/VertexLayout.h>
#include <SGL/Graphics/VertexArray.h>
#include <SGL/Graphics/UniformBuffer.h>
#include <SGL/Util/Memory.h>
#include <SGL/Runtime.h>

const char* VertexShaderSourceGL =
"#version 460 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aCol;\n"
"out vec3 oCol;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   oCol = aCol;\n"
"}\n";

const char* FragmentShaderSourceGL =
"#version 460 core\n"
"\n"
"out vec4 FragColor;\n"
"in vec3 oCol;\n"
"\n"
"layout(std140, binding=0) uniform ColourBuffer {"
"   vec3 c;"
"};"
"void main()\n"
"{\n"
"    FragColor = vec4(c, 1.0);\n"
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
"VSOutput main(VSInput input)\n"
"{\n"
"    VSOutput output;\n"
"    output.pos = float4(input.pos, 1.0f);\n"
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
"cbuffer ColourBuffer : register(b0) {"
"   float3 c; };"
"float4 main(PSInput input) : SV_TARGET\n"
"{\n"
"    return float4(c, 1.0f);\n"
"}\n";

typedef struct sgl_alignas(16) Vertex
{
    sgl_Vec3 vertex;
    sgl_Vec3 colour;
} Vertex;

typedef struct sgl_alignas(16) UB
{
    sgl_Vec3 colour;
} UB;

int main(int argc, char** argv)
{
    sgl_Runtime_Init();
    sgl_Memory_StackTrace(true);

    sgl_Logger_Init();

    sgl_EngineConfig cfg = sgl_EngineConfig_Default;
    cfg.backend = sgl_Backend_DIRECTX11;

    sgl_Window* window = sgl_Window_Create(cfg);
    sgl_GraphicsDevice* gpu = sgl_GraphicsDevice_Create(window);
    
    sgl_VertexLayout* layout = sgl_VertexLayout_New(gpu);
    {
        sgl_VertexElement pos =
        { 
            .semantic = sgl_POSITION,
            .offset = offsetof(Vertex, vertex),
            .type = sgl_VertexElementType_VEC3,
            .perInstance = false,
        };

        sgl_VertexElement col =
        {
            .semantic = sgl_COLOUR,
            .offset = offsetof(Vertex, colour),
            .type = sgl_VertexElementType_VEC3,
            .perInstance = false,
        };

        sgl_VertexLayout_Add(layout, pos);
        sgl_VertexLayout_Add(layout, col);
    }
    sgl_VertexArray* va = sgl_VertexArray_Create(gpu, sizeof(Vertex), layout);
    
    const float scale = 2.8f;

    const float x = 0.1333f * scale;
    const float y = 0.2370f * scale;

    sgl_Vec3 ptl = sgl_Vec3_New_ScalarXYZ(-x * 1.25f,  y, 0);
    sgl_Vec3 ptr = sgl_Vec3_New_ScalarXYZ( x,  y, 0);
    sgl_Vec3 pbr = sgl_Vec3_New_ScalarXYZ( x, -y, 0);
    sgl_Vec3 pbl = sgl_Vec3_New_ScalarXYZ(-x, -y, 0);

    Vertex tl;
    Vertex tr;
    Vertex bl;
    Vertex br;

    tl.vertex = ptl;
    tr.vertex = ptr;
    bl.vertex = pbl;
    br.vertex = pbr;

    tl.colour = sgl_Colour_ToVec3(sgl_Col_Red);
    tr.colour = sgl_Colour_ToVec3(sgl_Col_Green);
    bl.colour = sgl_Colour_ToVec3(sgl_Col_Blue);
    br.colour = sgl_Colour_ToVec3(sgl_Col_Yellow);

    sgl_VertexArray_Quad q = { .tl = &tl, .tr = &tr, .bl = &bl, .br = &br };
    sgl_VertexArray_AddQuad(va, q);

    sgl_Shader* shr;
    
    if (cfg.backend == sgl_Backend_DIRECTX11)
        shr = sgl_Shader_Create_Source(gpu, VertexShaderSourceDX, PixelShaderSourceDX, layout);
    else
        shr = sgl_Shader_Create_Source(gpu, VertexShaderSourceGL, FragmentShaderSourceGL, layout);

    UB uniform;
    uniform.colour = sgl_Colour_ToVec3(sgl_Col_Red);

    sgl_UniformBuffer* ub = sgl_UniformBuffer_Create(gpu, sizeof(UB));

    float h = 0.f;

    while (!window->wantsClose)
    {
        uniform.colour = sgl_Colour_ToVec3(sgl_Colour_FromHSV(sgl_Colour_Float(h, 1, 1)));
        sgl_UniformBuffer_Upload(ub, &uniform);
        h += 0.001f;

        sgl_Window_PollEvents(window);

        sgl_GraphicsDevice_Clear(gpu);

        sgl_GraphicsDevice_Draw(gpu, va, shr, &ub, 1);

        sgl_GraphicsDevice_Present(gpu);
    }

    sgl_UniformBuffer_Destroy(ub);
    sgl_VertexLayout_Destroy(layout);
    sgl_VertexArray_Destroy(va);
    sgl_Shader_Destroy(shr);
    sgl_GraphicsDevice_Destroy(gpu);
    sgl_Window_Destroy(window);
    sgl_Logger_Shutdown();
    sgl_Runtime_Shutdown();
    sgl_Memory_ReportLeaks();

    return 0;
}