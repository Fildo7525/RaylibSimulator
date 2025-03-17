set(SRC
	app.cpp
)

set(HEADERS
	app.h
)

add_library(app_lib
	${SRC}
	${HEADERS}
)

target_include_directories(
	app_lib
PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
	app_lib
PUBLIC
	object_lib
	tbb
)
