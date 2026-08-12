cmake_minimum_required(VERSION 3.10)
project(SYXZGameLibrary)

set(CMAKE_C_VER)

add_library(SGL SHARED
    src/imgui/backends/imgui_impl_dx11.cpp
    src/imgui/backends/imgui_impl_opengl3.cpp
    src/imgui/backends/imgui_impl_sdl3.cpp
    src/imgui/imgui.cpp
    src/imgui/imgui_demo.cpp
    src/imgui/imgui_draw.cpp
    src/imgui/imgui_tables.cpp
    src/imgui/imgui_widgets.cpp

    src/glad/glad.c
    src/osbridge.h
    src/osbridge.cpp
    src/HeaderOnlyImpl.cpp
    src/SGL/SGL.h
    src/SGL/Window.h
    src/SGL/Window.cpp
    src/SGL/Maths/Vec2.h
    src/SGL/Maths/Vec2.cpp
    src/SGL/Maths/Maths.h
    src/SGL/Maths/Maths.cpp
    src/SGL/Util/Memory.h
    src/SGL/Util/Memory.cpp
    src/SGL/Util/ANSI.h
    src/SGL/Util/ANSI.cpp
    src/SGL/Util/String.h
    src/SGL/Util/String.cpp
    src/SGL/Util/Logger.h
    src/SGL/Util/Logger.cpp
    src/SGL/Runtime.h
    src/SGL/Runtime.cpp
    src/SGL/Util/Error.h
    src/SGL/Util/Error.cpp
    src/SGL/Colour.h
    src/SGL/Colour.cpp
    src/SGL/Graphics/GraphicsDevice.h
    src/SGL/Graphics/Backends/OpenGL/GLDevice.h
    src/SGL/Graphics/Backends/OpenGL/GLDevice.cpp
    src/SGL/Graphics/GraphicsDevice.cpp
    src/SGL/Graphics/Shader.h
    src/SGL/Graphics/Backends/OpenGL/GLShader.h
    src/SGL/Graphics/Backends/OpenGL/GLShader.cpp
    src/SGL/Graphics/Shader.cpp
    
    src/SGL/Graphics/Backends/OpenGL/OpenGLThreadSync.h
    src/SGL/Graphics/Backends/OpenGL/OpenGLThreadSync.cpp
    src/SGL/Maths/Vec3.h
    src/SGL/Maths/Vec3.cpp
    src/SGL/Maths/Vec4.h
    src/SGL/Maths/Vec4.cpp
    src/SGL/Maths/Mat4.h
    src/SGL/Maths/Mat4.cpp
    src/SGL/Graphics/VertexLayout.h
    src/SGL/Graphics/VertexLayout.cpp
    src/SGL/Graphics/VertexArray.h
    src/SGL/Graphics/VertexArray.cpp
    src/SGL/Graphics/Backends/OpenGL/GLVertexArray.h
    src/SGL/Graphics/Backends/OpenGL/GLVertexArray.cpp
    src/SGL/Graphics/Backends/DirectX/DXDevice.h
    src/SGL/Graphics/Backends/DirectX/DXDevice.cpp
    src/SGL/Graphics/Backends/DirectX/DXShader.h
    src/SGL/Graphics/Backends/DirectX/DXShader.cpp
    src/SGL/Graphics/Backends/DirectX/DXVertexArray.h
    src/SGL/Graphics/Backends/DirectX/DXVertexArray.cpp
    src/SGL/Graphics/UniformBuffer.h
    src/SGL/Graphics/UniformBuffer.cpp
    src/SGL/Graphics/Backends/DirectX/DXUniformBuffer.h
    src/SGL/Graphics/Backends/DirectX/DXUniformBuffer.cpp
    src/SGL/Graphics/Backends/OpenGL/GLUniformBuffer.h
    src/SGL/Graphics/Backends/OpenGL/GLuniformBuffer.cpp
    src/SGL/Version.h
    src/SGL/Version.cpp
    src/SGL/ImGui/ImGui.h
    src/SGL/ImGui/ImGui.cpp
    src/SGL/Input/Key.h
    src/SGL/Input/Keyboard.h
    src/SGL/Input/Keyboard.cpp
    src/SGL/Graphics/Texture2D.h
    src/SGL/Graphics/Backends/DirectX/DXTexture2D.h
    src/SGL/Graphics/Texture2D.cpp
    src/SGL/Graphics/Backends/DirectX/DXTexture2D.cpp
    src/SGL/Graphics/Backends/OpenGL/GLTexture2D.h
    src/SGL/Graphics/Backends/OpenGL/GLTexture2D.cpp
    src/SGL/Graphics/Texture2DArray.h
    src/SGL/Graphics/Texture.h
    src/SGL/Graphics/Texture.cpp
    src/SGL/Graphics/Backends/OpenGL/GLTexture2DArray.h
    src/SGL/Graphics/Backends/OpenGL/GLTexture2DArray.cpp
    src/SGL/Graphics/Texture2DArray.cpp
    src/SGL/Graphics/Backends/DirectX/DXTexture2DArray.h
    src/SGL/Graphics/Backends/DirectX/DXTexture2DArray.cpp
    src/SGL/Graphics/PostProcess.h
    src/SGL/Graphics/Backends/OpenGL/GLPostProcess.h
    src/SGL/Graphics/Backends/OpenGL/GLPostProcess.cpp
    src/SGL/Graphics/PostProcess.cpp
    src/SGL/Graphics/Backends/DirectX/DXPostProcess.h
    src/SGL/Graphics/Backends/DirectX/DXPostProcess.cpp
    src/SGL/Input/MouseButton.h
    src/SGL/Input/Mouse.h
    src/SGL/Input/Mouse.cpp
    src/SGL/Graphics/Model.h
    src/SGL/Graphics/Model.cpp
)

include(CMake/Dependencies.cmake)

if (WIN32)
    target_link_libraries(SGL PRIVATE d3d11.lib d3dcompiler.lib)
endif ()

target_link_libraries(SGL PUBLIC SDL3::SDL3 glm::glm slang::gfx slang::slang)

target_compile_definitions(SGL PRIVATE SGL_EXPORTS)
target_compile_definitions(SGL PUBLIC UNICODE _UNICODE GLM_ENABLE_EXPERIMENTAL)

target_include_directories(SGL PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src)
target_include_directories(SGL PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/imgui)

target_compile_definitions(SGL PUBLIC -DSGL_PATH="${CMAKE_CURRENT_LIST_DIR}")

target_compile_options(SGL PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W3 /WX /MP>
        $<$<CXX_COMPILER_ID:Clang,GNU>:
        -Wall
        -Werror
        -Wno-unused-variable
        -Wno-unused-function
        >
)

target_compile_definitions(SGL PRIVATE $<$<PLATFORM_ID:Windows>:SGL_DIRECTX>)

# tiny_obj_loader.h's embedded fast_float (v8.0.2) detects C++20 constexpr
# algorithm support via __cpp_lib_constexpr_algorithms and marks some of its
# functions constexpr accordingly, but MSVC's constexpr-validation is
# stricter than that detection accounts for and fails to compile those
# functions under /std:c++latest (see fastfloat/fast_float upstream issues
# for this same MSVC incompatibility). Since that macro is only ever defined
# at C++20+, compiling this one file at C++17 makes fast_float's detection
# correctly fall back to its non-constexpr path instead - stb_image and
# tinyobjloader are both fine at C++17.
if (MSVC)
    set_source_files_properties(src/HeaderOnlyImpl.cpp PROPERTIES COMPILE_OPTIONS "/std:c++17")
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(SGL PRIVATE -D_DEBUG)
endif ()

set_target_properties(SGL PROPERTIES
	DEBUG_POSTFIX "-d"
)