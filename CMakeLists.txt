cmake_minimum_required(VERSION 3.21)
project(opengl_tutorial CXX)

find_package(glfw3)
find_package(glad)

add_subdirectory(src)
