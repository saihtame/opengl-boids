add_subdirectory(mesh)
add_subdirectory(material)

target_sources(particle_sim PRIVATE
    renderer.cpp
    mesh_instance.cpp
    cubemap.cpp
)
