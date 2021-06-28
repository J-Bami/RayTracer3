
project("Geometry")

file(GLOB GEOMETRY_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/*.h)
 
add_library(Geometry INTERFACE ${GEOMETRY_HEADERS})

target_link_libraries(Geometry INTERFACE Maths)

target_include_directories(Geometry INTERFACE .)