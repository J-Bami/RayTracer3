
add_subdirectory(MaterialComponents)
add_subdirectory(Surfaces)
add_subdirectory(LightSources)

project("Physics")
file(GLOB PHYSICS_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(Physics INTERFACE ${PHYSICS_HEADERS})

target_include_directories(Physics INTERFACE .)
target_link_libraries(Physics INTERFACE Optics MaterialComponents Surfaces LightSources Geometry Maths)
