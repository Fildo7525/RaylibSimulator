set(SRC
	loader.cpp
)

set(HEADERS
	loader.h
)

add_library(image_lib
	${SRC}
	${HEADERS}
)

target_include_directories(
	image_lib
PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
	image_lib
PUBLIC
	raylib
	nlohmann_json::nlohmann_json
)
