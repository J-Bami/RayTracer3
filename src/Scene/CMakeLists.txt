project("Scene")

file(GLOB SCENE_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(Scene INTERFACE ${SCENE_HEADERS})

target_include_directories(Scene INTERFACE .)
target_link_libraries(Scene INTERFACE Maths Containers Geometry Physics ${SDL2_LIBRARIES})