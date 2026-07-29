# SGL

SGL is a simple graphics library which abstracts DirectX and OpenGL calls, as well as providing an API to make graphics simple.
SGL, while implemented in C++, uses a C api allowing bindings to other languages.

## Features

- **Backend-agnostic graphics API**
- **Maths**
- **Obj model loading**
- **Input**
- **ImGui integration**
- **Slang integration**

## Requirements

- [vcpkg](https://github.com/microsoft/vcpkg) (Windows only ->) You must create an environment variable called `VCPKG_ROOT` and point it to your vcpkg install directory
- CMake 3.10+
- C++23 compiler

### Vcpkg packages

```
vcpkg install SDL3
vcpkg install glm
vcpkg install shader-slang
```

## Building

To build, you can use the command line
```
cmake -B out/build/x64-Debug -S .
```

or cmake fetch content
```cmake
include (FetchContent)
FetchContent_Declare(
    sgl
    GIT_REPOSITORY https://github.com/SYXZyt/SGL
    GIT_TAG <which ever version you need>
)

FetchContent_MakeAvailable(sgl)

target_link_libraries(<your target> SGL)
```

or as a directory

```cmake
add_subdirectory(<path to sgl>)
target_link_libraries(<your target> SGL)

```

## Shaders
Shaders can be written in the platforms' own shader langauge (GLSL/HLSL),
or preferably you can use Slang which means you can support multiple backends while
only writing one shader.

Note you'll have to use `[[vk::binding(...)]]` as well as registers to ensure GLSL support.
Check the example below.

## Quick example

A minimal window that clears to a colour and draws a textured quad, shaded with a single
Slang source that targets whichever backend is active:

```c
#include <SGL/Runtime.h>
#include <SGL/Window.h>
#include <SGL/Graphics/GraphicsDevice.h>
#include <SGL/Graphics/VertexLayout.h>
#include <SGL/Graphics/VertexArray.h>
#include <SGL/Graphics/Shader.h>
#include <SGL/Graphics/Texture2D.h>
#include <SGL/Maths/Vec2.h>
#include <SGL/Maths/Vec3.h>
#include <stddef.h>

typedef struct Vertex { sgl_Vec3 pos; sgl_Vec2 uv; } Vertex;

const char* SHADER =
"struct VSInput\n"
"{\n"
"    float3 pos : POSITION;\n"
"    float2 uv : TEXCOORD;\n"
"};\n"
"\n"
"struct VSOutput\n"
"{\n"
"    float4 pos : SV_Position;\n"
"    float2 uv : TEXCOORD;\n"
"};\n"
"\n"
"[[vk::binding(0)]]\n"
"Sampler2D gTex : register(t0);\n"
"\n"
"[shader(\"vertex\")]\n"
"VSOutput vertexMain(VSInput input)\n"
"{\n"
"    VSOutput output;\n"
"    output.pos = float4(input.pos, 1.0);\n"
"    output.uv = input.uv;\n"
"    return output;\n"
"}\n"
"\n"
"[shader(\"fragment\")]\n"
"float4 fragmentMain(VSOutput input) : SV_Target\n"
"{\n"
"    return gTex.Sample(input.uv);\n"
"}\n";

int main(void)
{
    sgl_Runtime_Init();

    sgl_EngineConfig cfg = sgl_EngineConfig_Default;
    sgl_Window* window = sgl_Window_Create(cfg);
    sgl_GraphicsDevice* gpu = sgl_GraphicsDevice_Create(window);

    sgl_VertexLayout* layout = sgl_VertexLayout_New(gpu);
    sgl_VertexElement pos = {
        .semantic = sgl_POSITION,
        .offset = offsetof(Vertex, pos),
        .type = sgl_VertexElementType_VEC3,
    };
    sgl_VertexElement uv = {
        .semantic = sgl_TEXCOORD,
        .offset = offsetof(Vertex, uv),
        .type = sgl_VertexElementType_VEC2,
    };
    sgl_VertexLayout_Add(layout, pos);
    sgl_VertexLayout_Add(layout, uv);

    sgl_VertexArray* quad = sgl_VertexArray_Create(gpu, sizeof(Vertex), layout);
    Vertex tl = { .pos = sgl_Vec3_New_ScalarXYZ(-0.5f,  0.5f, 0), .uv = sgl_Vec2_Up };
    Vertex tr = { .pos = sgl_Vec3_New_ScalarXYZ( 0.5f,  0.5f, 0), .uv = sgl_Vec2_One };
    Vertex bl = { .pos = sgl_Vec3_New_ScalarXYZ(-0.5f, -0.5f, 0), .uv = sgl_Vec2_Zero };
    Vertex br = { .pos = sgl_Vec3_New_ScalarXYZ( 0.5f, -0.5f, 0), .uv = sgl_Vec2_Right };
    sgl_VertexArray_Quad q = { .tl = &tl, .tr = &tr, .bl = &bl, .br = &br };
    sgl_VertexArray_AddQuad(quad, q);

    // Compiled once via Slang and translated to whichever backend sgl_GraphicsDevice picked
    sgl_Shader* shader = sgl_Shader_Create(gpu, layout);
    sgl_Shader_Load_Slang_Source(shader, SHADER, "vertexMain", "fragmentMain");

    sgl_Texture* texture = sgl_Texture2D_New_File(gpu, "texture.png");

    while (!window->wantsClose)
    {
        sgl_Window_PollEvents(window);

        sgl_GraphicsDevice_BeginFrame(gpu);
        sgl_GraphicsDevice_Draw(gpu, quad, shader, &texture, 1, NULL, 0);
        sgl_GraphicsDevice_EndFrame(gpu);
        sgl_GraphicsDevice_SwapBuffer(gpu);
    }

    sgl_Texture_Destroy(texture);
    sgl_Shader_Destroy(shader);
    sgl_VertexArray_Destroy(quad);
    sgl_VertexLayout_Destroy(layout);
    sgl_GraphicsDevice_Destroy(gpu);
    sgl_Window_Destroy(window);
    sgl_Runtime_Shutdown();
    return 0;
}
```

## Logging

SGL implements its own logger. You can provided your own log callback to handle the messages.
You cannot disable the logger, however you can pass an empty function instead. Passing `NULL` will reset to the built in log function