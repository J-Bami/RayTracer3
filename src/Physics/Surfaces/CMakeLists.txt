
project("Surfaces")


file(GLOB SURFACE_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(Surfaces INTERFACE ${SURFACE_HEADERS})

target_include_directories(Surfaces INTERFACE .)
target_link_libraries(Surfaces INTERFACE Maths Containers Optics MaterialComponents)