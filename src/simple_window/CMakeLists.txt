set(EXECUTABLE_NAME window)
add_executable(${EXECUTABLE_NAME} main.cpp)
target_link_libraries(${EXECUTABLE_NAME} glfw glad::glad)
