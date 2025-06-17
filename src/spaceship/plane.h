#pragma once

#include "object.h"
#include "quaternion.h"

class Plane
	: public rl::Object
{
public:
	static rl::Object::Ptr create(const rl::Model& model)
	{
		return std::make_shared<Plane>(model);
	}

	Plane(const rl::Model& model);
	~Plane();

	Vector6f getTorque() override;
};
