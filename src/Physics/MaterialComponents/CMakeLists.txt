project("MaterialComponents")


file(GLOB COMPONENT_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(MaterialComponents INTERFACE ${COMPONENT_HEADERS})
target_include_directories(MaterialComponents INTERFACE .)
target_link_libraries(MaterialComponents INTERFACE Maths Containers Optics)