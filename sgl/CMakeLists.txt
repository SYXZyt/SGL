cmake_minimum_required(VERSION 3.10)
project(SYXZGameLibrary)

add_library(sgl SHARED
    src/SGL/SGL.h
    src/SGL/Window.h
    src/SGL/Window.cpp
    src/SGL/Maths/Vec2.h
    src/SGL/Maths/Vec2.cpp
)

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

target_compile_definitions(sgl PRIVATE $<$<PLATFORM_ID:Windows>:SGL_DIRECTX>)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(sgl PRIVATE -D_DEBUG)
endif ()

set_target_properties(sgl PROPERTIES
	DEBUG_POSTFIX "-d"
)