set(SRC
	plane.cpp
)

set(HEADERS
	plane.h
)

add_library(plane_lib
	${SRC}
	${HEADERS}
)

target_include_directories(
	plane_lib
PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
	plane_lib
PUBLIC
	object_lib
	quat_lib
	tbb
)
