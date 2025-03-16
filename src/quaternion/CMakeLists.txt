set(SRC
	quaternion.cpp
)

set(HEADERS
	quaternion.h
)

add_library(quat_lib
	${SRC}
	${HEADERS}
)

target_include_directories(
	quat_lib
PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
	quat_lib
PUBLIC
	raylib
)
