
#THIS WAS HERE BEFORE
#add_library(Maths INTERFACE)

#target_include_directories(Maths INTERFACE .)


project ("Maths")

file(GLOB MATHS_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(Maths INTERFACE ${MATHS_HEADERS})
target_include_directories(Maths INTERFACE .)
