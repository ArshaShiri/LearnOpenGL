cmake_minimum_required(VERSION 3.15)
project(opengl_tutorial CXX)

find_package(glfw3)
find_package(glad)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} glfw glad::glad)
