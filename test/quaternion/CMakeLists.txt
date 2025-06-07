set(SRC
	test_quaternion.cpp
)

set(HEADERS
	test_quaternion.h
)

add_library(test_quat_lib
SHARED
	${SRC}
	${HEADERS}
)

add_compile_options( -fPIC )

target_include_directories(
	test_quat_lib
PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
	test_quat_lib
PUBLIC
	quat_lib
)
