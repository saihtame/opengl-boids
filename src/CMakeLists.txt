add_executable(particle_sim main.cpp)

add_subdirectory(core)
add_subdirectory(shaders)
add_subdirectory(render)
add_subdirectory(boids)
add_subdirectory(app)

target_include_directories(particle_sim PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(particle_sim PRIVATE
    OpenGL::GL
    SDL3::SDL3
    glad
    imgui
    stb::image
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(particle_sim PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

add_custom_command(TARGET particle_sim POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/shaders
            $<TARGET_FILE_DIR:particle_sim>/shaders
)

add_custom_command(TARGET particle_sim POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:particle_sim>/assets
)
