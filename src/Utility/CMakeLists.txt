project("Utility")

add_subdirectory(Maths)
add_subdirectory(Containers)
add_subdirectory(Geometry)

add_library(Utility INTERFACE)
target_include_directories(Utility INTERFACE .)

#target_link_libraries(Utility INTERFACE Maths Containers Geometry)