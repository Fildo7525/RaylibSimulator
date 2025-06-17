set(SRC
	spaceship.cpp
)

set(HEADERS
	spaceship.h
)

add_library(spaceship_lib
	${SRC}
	${HEADERS}
)

target_include_directories(
	spaceship_lib
PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
	spaceship_lib
PUBLIC
	object_lib
)
