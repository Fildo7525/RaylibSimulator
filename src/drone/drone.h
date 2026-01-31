#pragma once

#include "MovableObject.h"
#include "quaternion.h"

class Drone
	: public rl::MovableObject
{
public:
	static rl::Object::Ptr create(const rl::Model& model)
	{
		return std::shared_ptr<Drone>(new Drone(model));
	}

	Vector6f getTorque();
	Vector6f getTorqueKeyboard();
	Vector6f getTorqueGamepad();

	~Drone();

private:
	explicit Drone(const rl::Model& model);
};
