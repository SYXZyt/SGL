cmake_minimum_required(VERSION 3.12)
project(SYXZGameLibrary)

add_executable(Test "main.c")

target_link_libraries(Test PRIVATE SGL)

file(GLOB SGL_TEST_ASSETS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.slang"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.obj"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.png"
)

set(SGL_TEST_ASSETS_STAMP "${CMAKE_CURRENT_BINARY_DIR}/assets.stamp")

add_custom_command(
    OUTPUT "${SGL_TEST_ASSETS_STAMP}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SGL_TEST_ASSETS} "${CMAKE_BINARY_DIR}"
    COMMAND ${CMAKE_COMMAND} -E touch "${SGL_TEST_ASSETS_STAMP}"
    DEPENDS ${SGL_TEST_ASSETS}
    COMMENT "Copying test assets to output directory"
)

add_custom_target(CopyTestAssets ALL DEPENDS "${SGL_TEST_ASSETS_STAMP}")
add_dependencies(Test CopyTestAssets)