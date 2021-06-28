project("File")


file(GLOB FILE_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(File INTERFACE ${FILE_HEADERS})
target_include_directories(File INTERFACE .)
#target_link_libraries(File INTERFACE)