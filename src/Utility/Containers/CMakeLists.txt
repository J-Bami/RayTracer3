
project("Containers")

file(GLOB CONTAINERS_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(Containers INTERFACE ${CONTAINERS_HEADERS})
target_include_directories(Containers INTERFACE .)
#target_link_libraries(Containers INTERFACE Utility)