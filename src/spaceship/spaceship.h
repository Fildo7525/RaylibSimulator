#pragma once

#include "MovableObject.h"
#include "quaternion.h"

class Spaceship
	: public rl::MovableObject
{
public:
	static rl::Object::Ptr create(const rl::Model& model)
	{
		return std::shared_ptr<Spaceship>(new Spaceship(model));
	}

	Vector6f getTorque() override;
	~Spaceship();

private:
	Spaceship(const rl::Model& model);
};
