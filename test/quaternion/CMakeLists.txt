set(SRC
	quaternion.cpp
)

set(HEADERS
	quaternion.h
)

add_library(test_quat_lib
SHARED
	${SRC}
	${HEADERS}
)

target_link_libraries(
	test_quat_lib
PUBLIC
	quat_lib
)
