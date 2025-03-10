#include "quaternion.h"
#include "../../src/quaternion/quaternion.h"

void test_quaternion()
{
	rlc::Quaternion q(1, 2, 3, 4);
	std::println("Quaternion: {}", q);
	std::println("Magnitude: {}", q.magnitude());
	std::println("Normalized quaternion: {}", q.normalize());
	std::println("Magnitude: {}", q.magnitude());

}
