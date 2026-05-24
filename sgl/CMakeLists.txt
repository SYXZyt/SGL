cmake_minimum_required(VERSION 3.10)
project(SYXZGameLibrary)

add_library(SGL SHARED
    src/osbridge.h
    src/osbridge.cpp
    src/SGL/SGL.h
    src/SGL/Window.h
    src/SGL/Window.cpp
    src/SGL/Maths/Vec2.h
    src/SGL/Maths/Vec2.cpp
    src/SGL/Maths/Maths.h
    src/SGL/Maths/Maths.cpp
    src/SGL/Util/Memory.h
    src/SGL/Util/Memory.cpp
 "src/SGL/Util/ANSI.h" "src/SGL/Util/String.h" "src/SGL/Util/String.cpp" "src/SGL/Util/ANSI.cpp")

include(CMake/Dependencies.cmake)

if (WIN32)
    target_link_libraries(SGL PRIVATE d3d11.lib d3dcompiler.lib)
endif ()

target_link_libraries(SGL PUBLIC SDL3::SDL3)

target_compile_definitions(SGL PRIVATE SGL_EXPORTS)
target_compile_definitions(SGL PUBLIC UNICODE _UNICODE GLM_ENABLE_EXPERIMENTAL)

target_include_directories(SGL PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src)

target_compile_definitions(SGL PUBLIC -DSGL_PATH="${CMAKE_CURRENT_LIST_DIR}")

target_compile_options(SGL PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W3 /WX /MP>
    $<$<CXX_COMPILER_ID:Clang,GNU>:-Wall -Werror>
)

target_compile_definitions(SGL PRIVATE $<$<PLATFORM_ID:Windows>:SGL_DIRECTX>)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(SGL PRIVATE -D_DEBUG)
endif ()

set_target_properties(SGL PROPERTIES
	DEBUG_POSTFIX "-d"
)