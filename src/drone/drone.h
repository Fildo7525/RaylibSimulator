#pragma once

#include "MovableObject.h"
#include "quaternion.h"

class Drone
	: public rl::MovableObject
{
public:
	static rl::Object::Ptr create(const rl::Model& model)
	{
		return std::make_shared<Drone>(model);
	}

	explicit Drone(const rl::Model& model);
	~Drone();

	Vector6f getTorque();
	Vector6f getTorqueKeyboard();
	Vector6f getTorqueGamepad();
};
