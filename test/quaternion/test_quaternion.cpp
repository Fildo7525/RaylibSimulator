#include <cassert>
#include "test_quaternion.h"

void test_quaternion()
{

	auto q = rl::Quaternion::fromEuler(0, PI/2., 0);
	q = q.normalize();

	auto rlq = QuaternionFromEuler(0, PI/2., 0);

	std::println("Quaternion: {}", q);
	std::println("Quaternion from raylib: ({}, {}, {}, {})", rlq.x, rlq.y, rlq.z, rlq.w);

	// assert(rlq.x == q.x());
	// assert(rlq.y == q.y());
	// assert(rlq.z == q.z());
	// assert(rlq.w == q.w());

	std::println("Magnitude: {}", q.magnitude());
	std::println("Normalized quaternion: {}", q.normalize());
	std::println("Magnitude: {}", q.magnitude());

	Vector3f euler{PI/2., 0, 0};
	std::println("Euler Angles: ({}, {}, {})", euler.x(), euler.y(), euler.z());

	rl::Quaternion q2 = rl::Quaternion::fromEuler(euler);
	std::println("Quaternion: {}", q2);

	auto R = q2.toRotationMatrix();
	std::println("Rotation Matrix: \n{}", R);
}
