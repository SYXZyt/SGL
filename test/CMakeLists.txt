cmake_minimum_required(VERSION 3.10)
project(SYXZGameLibrary)

add_executable(Test "main.cpp")

target_link_libraries(Test PRIVATE SGL)