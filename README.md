# SGL

A small 2D/3D graphics library for Windows, with interchangeable OpenGL 4.6 and
DirectX 11 backends behind a single C API. It's a solo, actively-developed
hobby project — expect the API to keep moving.

Current version: **Beta 0.4.0**

## Features

- **Backend-agnostic graphics API** — windows, devices, shaders, vertex
  arrays (with optional index buffers), uniform buffers, 2D/2D-array
  textures, and post-processing effects, all through one `sgl_GraphicsDevice`
  interface backed by either OpenGL or DirectX 11.
- **Maths** — `Vec2`/`Vec2i`/`Vec3`/`Vec3i`/`Vec4`/`Mat4`, plus a
  `sgl::Maths::` C++ namespace (operator overloads, overload-resolved helpers
  like `Dist`/`Cross`/`Normalise`) layered on top of the same C functions for
  callers who don't need a pure-C ABI.
- **Model loading** — `sgl_Model_Load` reads an OBJ file (via `tinyobjloader`)
  straight into an indexed `sgl_VertexArray`, packed to match whatever vertex
  layout (position/normal/texcoord) you give it.
- **Input** — keyboard and mouse (buttons, position, delta, scroll wheel,
  relative/captured mode for FPS-style look controls).
- **ImGui integration** — a thin C wrapper around Dear ImGui for both
  backends.
- **Depth testing** — on by default, toggle it off at runtime
  (`sgl_GraphicsDevice_SetDepthTestEnabled`) if you want 2D draw-order-based
  layering instead of a depth buffer.

## Requirements

- Windows, Visual Studio 2022 (or another MSVC-compatible toolchain)
- [vcpkg](https://github.com/microsoft/vcpkg), with `VCPKG_ROOT` set — used to
  fetch `SDL3` and `glm`
- CMake 3.10+
- A C++23-capable compiler for the library itself; consuming code can be
  plain C11 (the whole public API is `extern "C"`)

DirectX 11 support is Windows-only and requires `d3d11.lib`/`d3dcompiler.lib`
(present with any standard Windows SDK install); the OpenGL backend works
without them.

## Building

```
git clone <this repo>
cd SGL
cmake -B out/build/x64-Debug -S . -DCMAKE_BUILD_TYPE=Debug
cmake --build out/build/x64-Debug
```

This builds two targets:
- **SGL** — the library itself, `SGL-d.dll`/`SGL-d.lib` in Debug (`SGL.dll` in
  Release)
- **Test** — a small demo app (`test/main.c`) exercising the library: a
  textured, lit, indexed cube and ground plane, an FPS camera, mouse-look,
  and a post-processing effect

Run `Test.exe` from the build output directory (it loads `test/stone.png`
relative to its working directory).

## Quick example

A minimal window that clears to a colour and draws a flat-shaded quad:

```c
#include <SGL/Runtime.h>
#include <SGL/Window.h>
#include <SGL/Graphics/GraphicsDevice.h>
#include <SGL/Graphics/VertexLayout.h>
#include <SGL/Graphics/VertexArray.h>
#include <SGL/Graphics/Shader.h>
#include <stddef.h>

typedef struct Vertex { sgl_Vec3 pos; } Vertex;

const char* VS =
"#version 460 core\n"
"layout(location=0) in vec3 aPos;\n"
"void main() { gl_Position = vec4(aPos, 1.0); }\n";

const char* FS =
"#version 460 core\n"
"out vec4 FragCol;\n"
"void main() { FragCol = vec4(0.2, 0.6, 1.0, 1.0); }\n";

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
    sgl_VertexLayout_Add(layout, pos);

    sgl_VertexArray* quad = sgl_VertexArray_Create(gpu, sizeof(Vertex), layout);
    Vertex tl = { .pos = sgl_Vec3_New_ScalarXYZ(-0.5f,  0.5f, 0) };
    Vertex tr = { .pos = sgl_Vec3_New_ScalarXYZ( 0.5f,  0.5f, 0) };
    Vertex bl = { .pos = sgl_Vec3_New_ScalarXYZ(-0.5f, -0.5f, 0) };
    Vertex br = { .pos = sgl_Vec3_New_ScalarXYZ( 0.5f, -0.5f, 0) };
    sgl_VertexArray_Quad q = { .tl = &tl, .tr = &tr, .bl = &bl, .br = &br };
    sgl_VertexArray_AddQuad(quad, q);

    sgl_Shader* shader = sgl_Shader_Create(gpu, layout);
    sgl_Shader_Load_Source(shader, VS, FS);

    while (!window->wantsClose)
    {
        sgl_Window_PollEvents(window);

        sgl_GraphicsDevice_BeginFrame(gpu);
        sgl_GraphicsDevice_Draw(gpu, quad, shader, NULL, 0, NULL, 0);
        sgl_GraphicsDevice_EndFrame(gpu);
        sgl_GraphicsDevice_SwapBuffer(gpu);
    }

    sgl_Shader_Destroy(shader);
    sgl_VertexArray_Destroy(quad);
    sgl_VertexLayout_Destroy(layout);
    sgl_GraphicsDevice_Destroy(gpu);
    sgl_Window_Destroy(window);
    sgl_Runtime_Shutdown();
    return 0;
}
```

To target DirectX 11 instead, set `cfg.backend = sgl_Backend_DIRECTX11;`
before creating the window, and load HLSL source via the same
`sgl_Shader_Load_Source` call — see `test/main.c` for a real example that
compiles shaders for both backends side by side.

## Module overview

| Module | Path | What it covers |
|---|---|---|
| Core | `SGL/src/SGL/SGL.h`, `Runtime.h`, `Window.h` | Platform macros, engine init/shutdown, SDL3-backed windowing |
| Graphics | `SGL/src/SGL/Graphics/` | `GraphicsDevice`, `Shader`, `VertexArray`/`VertexLayout`, `UniformBuffer`, `Texture2D(Array)`, `PostProcess`, `Model` (OBJ loading) |
| Backends | `SGL/src/SGL/Graphics/Backends/{OpenGL,DirectX}/` | Concrete GL 4.6 and D3D11 implementations behind the `Graphics` interfaces |
| Maths | `SGL/src/SGL/Maths/` | `Vec2/Vec3/Vec4/Mat4`, scalar/vector helpers, plus the `sgl::Maths::` C++ namespace |
| Input | `SGL/src/SGL/Input/` | Keyboard and mouse |
| ImGui | `SGL/src/SGL/ImGui/` | Wrapper over the vendored Dear ImGui |
| Util | `SGL/src/SGL/Util/` | Memory tracking, logging, string/error helpers |

Vendored third-party code (Dear ImGui, GLAD, stb_image, tinyobjloader) lives
under `SGL/src/` alongside first-party code — see each vendored file's own
header for its license.

## Logging

`sgl_Log`/`sgl_LogWarning`/`sgl_LogError`/`sgl_LogSuccess` write to an
internal async logger that prints to the console by default. Call
`sgl_Logger_SetCallback` to redirect messages to your own sink instead (e.g.
to integrate with an existing logging system).

## Status

This is under active, exploratory development with no stability guarantees —
expect breaking API changes between versions. There is currently no
automated test suite; correctness is largely verified by hand against
`test/main.c` and ad hoc scratch programs.
