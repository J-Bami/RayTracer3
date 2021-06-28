project("Optics")

file(GLOB OPTICS_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(Optics INTERFACE ${OPTICS_HEADERS})
target_include_directories(Optics INTERFACE .)
target_link_libraries(Optics INTERFACE Maths Containers)