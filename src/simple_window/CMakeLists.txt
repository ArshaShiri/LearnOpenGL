set(EXECUTABLE_NAME window)

add_executable(${EXECUTABLE_NAME}
    ../common/window.cpp
    main.cpp
)

target_link_libraries(${EXECUTABLE_NAME} glfw glad::glad)
target_include_directories(${EXECUTABLE_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/src")
