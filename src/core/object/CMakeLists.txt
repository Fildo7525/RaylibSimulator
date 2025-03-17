set(SRC
	object.cpp
)

set(HEADERS
	object.h
)

add_library(object_lib
	${SRC}
	${HEADERS}
)

target_include_directories(
	object_lib
PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
	object_lib
PUBLIC
	image_lib
)
