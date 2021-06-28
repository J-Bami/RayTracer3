
add_subdirectory(Utility)
add_subdirectory(File)
add_subdirectory(Optics)
add_subdirectory(Physics)
add_subdirectory(Scene)

add_executable (RayTracer3 "RayTracer3.cpp")
target_link_libraries(RayTracer3 PUBLIC Utility Optics Physics ${SDL2_LIBRARIES})
#target_link_libraries(RayTracer3 )
# TODO: Add tests and install targets if needed.