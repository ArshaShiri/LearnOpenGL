cmake_minimum_required(VERSION 3.21)
project(opengl_tutorial CXX)

find_package(glfw3)
find_package(glad)

# This is going to be useful to link to the VS Code.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)
