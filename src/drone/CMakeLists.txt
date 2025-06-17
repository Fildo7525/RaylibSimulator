set(SRC
	drone.cpp
)

set(HEADERS
	drone.h
)

add_library(drone_lib
	${SRC}
	${HEADERS}
)

target_include_directories(
	drone_lib
PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
	drone_lib
PUBLIC
	object_lib
)
