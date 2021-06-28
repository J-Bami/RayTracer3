project("LightSources")


file(GLOB LIGHT_SOURCE_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(LightSources INTERFACE ${LIGHT_SOURCE_HEADERS})
target_include_directories(LightSources INTERFACE .)
target_link_libraries(LightSources INTERFACE Maths Containers Optics Surfaces)