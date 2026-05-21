cmake_minimum_required(VERSION 3.10)
project(SYXZGameLibrary)

add_executable(Test "main.c")

target_link_libraries(Test PRIVATE sgl)