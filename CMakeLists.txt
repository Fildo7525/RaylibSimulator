cmake_minimum_required(VERSION 3.31.2)
project(first VERSION 3.31.2)

include(FetchContent)

FetchContent_Declare(
	json
	GIT_REPOSITORY https://github.com/nlohmann/json.git
	# GIT_TAG 55f93686c01528224f448c19128836e7df245f72 # v3.12.0
	GIT_TAG v3.12.0
	SYSTEM
)

FetchContent_MakeAvailable(json)

set(CMAKE_CXX_COMPILER /usr/bin/g++-14)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)


add_definitions(-DPLANE_CONFIG_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/plane.json")
add_definitions(-DDRONE_CONFIG_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/drone.json")
add_definitions(-DSPACESHIP_CONFIG_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/spaceship.json")

find_package(raylib 5.5 REQUIRED)
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
	drone_lib
	plane_lib
)

