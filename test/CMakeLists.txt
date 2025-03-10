set(SRC
	main.cpp
)

add_subdirectory(quaternion)

add_executable(test
	${SRC}
)

target_link_libraries(
	test
PUBLIC
	quat_lib
	test_quat_lib
)
