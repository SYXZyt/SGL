cmake_minimum_required(VERSION 3.10)

if (DEFINED ENV{VCPKG_ROOT})
	set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
	set(VCPKG_TARGET_TRIPLET "x64-windows")
	message("Using toolchain ${CMAKE_TOOLCHAIN_FILE}")
endif ()

project("SYXZGameLibrary")

set(sgl_root ${CMAKE_CURRENT_LIST_DIR})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

if (WIN32)
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif ()

add_subdirectory(sgl)
add_subdirectory(test)