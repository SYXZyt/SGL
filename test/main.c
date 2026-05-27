#include <SGL/Window.h>
#include <SGL/Maths/Vec2.h>
#include <SGL/Util/ANSI.h>
#include <stdio.h>
#include <SGL/Util/Logger.h>
#include <SGL/Graphics/GraphicsDevice.h>
#include <SGL/Graphics/Shader.h>
#include <SGL/Graphics/VertexLayout.h>

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

int main(int argc, char** argv)
{
    sgl_Logger_Init();

    sgl_EngineConfig cfg = sgl_EngineConfig_Default;
    sgl_Window* window = sgl_Window_Create(cfg);
    sgl_GraphicsDevice* gpu = sgl_GraphicsDevice_Create(window);
    sgl_VertexLayout* vertexLayout = sgl_VertexLayout_New(gpu);
    sgl_VertexLayout_Add(vertexLayout, sgl_VertexElementType_FLOAT3);
    sgl_VertexLayout_Bind(vertexLayout);


    sgl_Shader* shr = sgl_Shader_Create_Source(gpu, VertexShaderSource, FragmentShaderSource);

    while (!window->wantsClose)
    {
        sgl_Window_PollEvents(window);

        sgl_GraphicsDevice_Clear(gpu, sgl_ClearFlag_COLOUR | sgl_ClearFlag_DEPTH);
        sgl_Shader_Bind(shr);
        sgl_GraphicsDevice_Present(gpu);
    }

    sgl_Shader_Destroy(shr);
    sgl_GraphicsDevice_Destroy(gpu);
    sgl_Window_Destroy(window);
    sgl_Logger_Shutdown();

    return 0;
}