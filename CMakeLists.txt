cmake_minimum_required(VERSION 3.20)

project(particle_sim VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)
find_package(SDL3 REQUIRED CONFIG REQUIRED)

add_subdirectory(external/glad)
add_subdirectory(external/glm)
add_subdirectory(external/stb)
add_subdirectory(external/imgui)
add_subdirectory(src)
