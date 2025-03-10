cmake_minimum_required(VERSION 3.31.2)
project(first VERSION 3.31.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

set(SOURCES
	main.cpp
)
set(HEADERS
)

add_subdirectory(src)

add_executable(${PROJECT_NAME}
	${SOURCES}
	${HEADERS}
)

target_link_libraries(
	${PROJECT_NAME}
PUBLIC
	app_lib
	quat_lib
)

