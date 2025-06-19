#pragma once

#include "object.h"
#include "quaternion.h"

class Spaceship
	: public rl::Object
{
public:
	static rl::Object::Ptr create(const rl::Model& model)
	{
		return std::make_shared<Spaceship>(model);
	}

	Spaceship(const rl::Model& model);
	~Spaceship();

	Vector6f getTorque() override;
};
