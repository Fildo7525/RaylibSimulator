set(SRC
	pid.cpp
)

set(HEADERS
	pid.h
)

add_library(pid
	${SRC}
	${HEADERS}
)

target_include_directories(
	pid
PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)
