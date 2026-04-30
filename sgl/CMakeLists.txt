cmake_minimum_required(VERSION 3.10)
project(SYXZGameLibrary)

add_library(sgl SHARED
    src/sgl/sgl.h
    src/sgl/String.h
    src/sgl/String.cpp
    src/sgl/Maths/Maths.cpp
    src/sgl/Maths/Vec2.cpp
    src/sgl/Memory/Memory.cpp
 "src/sgl/Memory/StlAllocator.h" "src/sgl/Collections/List.h" "src/sgl/FileSystem.cpp" "src/sgl/Error.h" "src/sgl/Error.cpp")

include(CMake/Dependencies.cmake)

if (WIN32)
    target_link_libraries(sgl PRIVATE d3d11.lib d3dcompiler.lib)
endif ()

target_link_libraries(sgl PUBLIC SDL3::SDL3)

target_compile_definitions(sgl PRIVATE SGL_EXPORTS)
target_compile_definitions(sgl PUBLIC UNICODE _UNICODE GLM_ENABLE_EXPERIMENTAL)

target_include_directories(sgl PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src)

target_compile_definitions(sgl PUBLIC -DSGL_PATH="${CMAKE_CURRENT_LIST_DIR}")

target_compile_options(sgl PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W3 /WX /MP>
    $<$<CXX_COMPILER_ID:Clang,GNU>:-Wall -Werror>
)