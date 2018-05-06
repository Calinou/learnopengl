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

# Coordinate Systems
add_executable(CoordinateSystems src/CoordinateSystems.cpp src/Shader.hpp)
target_link_libraries(CoordinateSystems ${LIBRARIES})

# Camera
add_executable(Camera src/Camera.cpp src/Shader.hpp)
target_link_libraries(Camera ${LIBRARIES})

# Lights (Colors)
add_executable(LightColors src/LightColors.cpp src/Shader.hpp)
target_link_libraries(LightColors ${LIBRARIES})

# Basic Lighting
add_executable(BasicLighting src/BasicLighting.cpp src/Shader.hpp)
target_link_libraries(BasicLighting ${LIBRARIES})

# Basic Lighting (Gouraud shading)
add_executable(BasicLightingGouraud src/BasicLightingGouraud.cpp src/Shader.hpp)
target_link_libraries(BasicLightingGouraud ${LIBRARIES})

# Materials
add_executable(Materials src/Materials.cpp src/Shader.hpp)
target_link_libraries(Materials ${LIBRARIES})

# Lighting Maps
add_executable(LightingMaps src/LightingMaps.cpp src/Shader.hpp)
target_link_libraries(LightingMaps ${LIBRARIES})

# Light Casters (Directional)
add_executable(LightCastersDirectional src/LightCastersDirectional.cpp src/Shader.hpp)
target_link_libraries(LightCastersDirectional ${LIBRARIES})

# Light Casters (Point)
add_executable(LightCastersPoint src/LightCastersPoint.cpp src/Shader.hpp)
target_link_libraries(LightCastersPoint ${LIBRARIES})

# Light Casters Spot
add_executable(LightCastersSpot src/LightCastersSpot.cpp src/Shader.hpp)
target_link_libraries(LightCastersSpot ${LIBRARIES})

# Multiple Lights
add_executable(MultipleLights src/MultipleLights.cpp src/Shader.hpp)
target_link_libraries(MultipleLights ${LIBRARIES})
