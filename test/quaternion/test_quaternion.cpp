#include "quaternion.h"
#include <cassert>
#include <raymath.h>
#include "test_quaternion.h"

void test_quaternion()
{
	rl::Quaternion q(1, 2, 3, 4);
	q = q.normalize();

	auto rlq = QuaternionFromEuler(2, 3, 1);

	std::println("Quaternion: {}", q);
	std::println("Quaternion from raylib: ({}, {}, {}, {})", rlq.x, rlq.y, rlq.z, rlq.w);

	assert(rlq.x == q.data()[0]);
	assert(rlq.y == q.data()[1]);
	assert(rlq.z == q.data()[2]);
	assert(rlq.w == q.data()[3]);

	std::println("Magnitude: {}", q.magnitude());
	std::println("Normalized quaternion: {}", q.normalize());
	std::println("Magnitude: {}", q.magnitude());

}
