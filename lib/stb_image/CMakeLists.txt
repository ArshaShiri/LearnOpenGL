cmake_minimum_required(VERSION 3.15)
project(stb_image CXX)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PRIVATE src/stb_image.cpp)
target_include_directories(stb_image PUBLIC include)


file(GLOB_RECURSE INCLUDE_FILES "include/*.h*")
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    PUBLIC_HEADER "${INCLUDE_FILES}"
)

install(TARGETS ${PROJECT_NAME} PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
