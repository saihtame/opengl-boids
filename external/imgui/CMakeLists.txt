cmake_minimum_required(VERSION 3.16)

project(imgui LANGUAGES CXX)

add_library(imgui STATIC
    imgui.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui_demo.cpp
    backends/imgui_impl_sdl3.cpp
    backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/backends
)

find_package(SDL3 REQUIRED CONFIG)
find_package(OpenGL REQUIRED)

target_link_libraries(imgui
    PUBLIC
        SDL3::SDL3
        OpenGL::OpenGL
)

target_compile_features(imgui PUBLIC cxx_std_17)

if (WIN32)
    target_link_libraries(imgui PUBLIC opengl32)
endif()
