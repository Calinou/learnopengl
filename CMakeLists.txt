cmake_minimum_required(VERSION 3.10)
project(learnopengl)
set(CMAKE_CXX_STANDARD 17)

# Libraries
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
add_library(glad STATIC lib/glad/glad.c)
include_directories(include)
include_directories(${GLFW_INCLUDE_DIRS})

# Hello Rectangle
add_executable(hello_rectangle hello_rectangle.cpp)
target_link_libraries(hello_rectangle ${GLFW_LIBRARIES} glad dl)

# Hello Rectangle (2 colors)
add_executable(hello_rectangle_2 hello_rectangle_2.cpp)
target_link_libraries(hello_rectangle_2 ${GLFW_LIBRARIES} glad dl)

# Shader Uniforms
add_executable(shaders_uniforms shaders_uniforms.cpp)
target_link_libraries(shaders_uniforms ${GLFW_LIBRARIES} glad dl)

# Shader VAOs
add_executable(shaders_vao shaders_vao.cpp)
target_link_libraries(shaders_vao ${GLFW_LIBRARIES} glad dl)
