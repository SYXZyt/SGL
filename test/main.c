#include <SGL/Window.h>
#include <SGL/Maths/Vec3.h>
#include <SGL/Util/ANSI.h>
#include <stdio.h>
#include <SGL/Util/Logger.h>
#include <SGL/Graphics/GraphicsDevice.h>
#include <SGL/Graphics/Shader.h>
#include <SGL/Graphics/VertexLayout.h>
#include <SGL/Graphics/VertexArray.h>

const char* VertexShaderSource =
"#version 330 core\n"
"\n"
"layout(location = 0) in vec3 aPos;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos, 1.0);\n"
"}\n";

const char* FragmentShaderSource =
"#version 330 core\n"
"\n"
"out vec4 FragColor;\n"
"\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0, 0.2, 0.4, 1.0);\n"
"}\n";

typedef struct Vertex
{
    sgl_Vec3 vertex;
} Vertex;

int main(int argc, char** argv)
{
    sgl_Logger_Init();

    sgl_EngineConfig cfg = sgl_EngineConfig_Default;
    sgl_Window* window = sgl_Window_Create(cfg);
    sgl_GraphicsDevice* gpu = sgl_GraphicsDevice_Create(window);
    
    sgl_VertexLayout* layout = sgl_VertexLayout_New(gpu);
    {
        sgl_VertexElement pos =
        {
            .semantic = sgl_MakeString("POSITION"),
            .offset = 0,
            .type = sgl_VertexElementType_VEC3,
            .perInstance = false,
        };

        sgl_VertexLayout_Add(layout, pos);
    }
    sgl_VertexArray* va = sgl_VertexArray_Create(gpu, sizeof(Vertex), layout);
    
    const float x = 0.1333f;
    const float y = 0.2370f;

    sgl_Vec3 tl = sgl_Vec3_New_ScalarXYZ(-x, -y, 0);
    sgl_Vec3 tr = sgl_Vec3_New_ScalarXYZ( x, -y, 0);
    sgl_Vec3 br = sgl_Vec3_New_ScalarXYZ( x,  y, 0);
    sgl_Vec3 bl = sgl_Vec3_New_ScalarXYZ(-x,  y, 0);

    sgl_VertexArray_Quad q = { .tl = &tl, .tr = &tr, .bl = &bl, .br = &br };
    sgl_VertexArray_AddQuad(va, q);

    sgl_Shader* shr = sgl_Shader_Create_Source(gpu, VertexShaderSource, FragmentShaderSource);

    while (!window->wantsClose)
    {
        sgl_Window_PollEvents(window);

        sgl_GraphicsDevice_Clear(gpu, sgl_ClearFlag_COLOUR | sgl_ClearFlag_DEPTH);

        sgl_GraphicsDevice_Draw(gpu, va, shr);

        sgl_GraphicsDevice_Present(gpu);
    }

    sgl_VertexArray_Destroy(va);
    sgl_Shader_Destroy(shr);
    sgl_GraphicsDevice_Destroy(gpu);
    sgl_Window_Destroy(window);
    sgl_Logger_Shutdown();

    return 0;
}