cmake_minimum_required(VERSION 3.31.2)
project(first VERSION 3.31.2)

set(CMAKE_CXX_COMPILER /usr/bin/g++-14)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

add_definitions(-DPLANE_MODEL_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/model/plane.obj")
add_definitions(-DDRONE_MODEL_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/model/drone_body.obj")
add_definitions(-DPLANE_TEXTURE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/texture/plane_diffuse.png")

find_package(raylib 3.0 REQUIRED)
find_package(Eigen3 3.4 REQUIRED)

set(SOURCES
	main.cpp
)
set(HEADERS
)

add_subdirectory(src)
add_subdirectory(test)

add_executable(${PROJECT_NAME}
	${SOURCES}
	${HEADERS}
)

target_link_libraries(
	${PROJECT_NAME}
PUBLIC
	app_lib
	plane_lib
)

