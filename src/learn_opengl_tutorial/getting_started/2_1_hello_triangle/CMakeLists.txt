project(hello_triangle)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src/common/program.cpp
    ${CMAKE_SOURCE_DIR}/src/common/index_buffer.cpp
    ${CMAKE_SOURCE_DIR}/src/common/renderer.cpp
    ${CMAKE_SOURCE_DIR}/src/common/shader.cpp
    ${CMAKE_SOURCE_DIR}/src/common/vertex_array.cpp
    ${CMAKE_SOURCE_DIR}/src/common/vertex_buffer.cpp
    ${CMAKE_SOURCE_DIR}/src/common/window.cpp
    main.cpp
)

target_link_libraries(${PROJECT_NAME} glfw glm::glm glad::glad)
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/src")
