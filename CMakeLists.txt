cmake_minimum_required(VERSION 3.10)
project(learnopengl)
set(CMAKE_CXX_STANDARD 17)

# Libraries
find_package(PkgConfig REQUIRED)
find_package(glm REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
add_library(glad STATIC lib/glad/glad.c)
include_directories(include)
include_directories(${GLFW_INCLUDE_DIRS})
set(LIBRARIES ${GLFW_LIBRARIES} glad glm dl)

# Hello Rectangle
add_executable(HelloRectangle src/HelloRectangle.cpp)
target_link_libraries(HelloRectangle ${LIBRARIES})

# Hello Rectangle (2 colors)
add_executable(HelloRectangle2 src/HelloRectangle2.cpp)
target_link_libraries(HelloRectangle2 ${LIBRARIES})

# Shader Uniforms
add_executable(ShaderUniforms src/ShaderUniforms.cpp)
target_link_libraries(ShaderUniforms ${LIBRARIES})

# Shader VAOs
add_executable(ShadersVAO src/ShadersVAO.cpp src/Shader.hpp)
target_link_libraries(ShadersVAO ${LIBRARIES})

# Textures
add_executable(Textures src/Textures.cpp src/Shader.hpp)
target_link_libraries(Textures ${LIBRARIES})

# Transforms
add_executable(Transforms src/Transforms.cpp src/Shader.hpp)
target_link_libraries(Transforms ${LIBRARIES})
