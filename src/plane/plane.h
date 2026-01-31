#pragma once

#include "MovableObject.h"
#include "quaternion.h"

class Plane
	: public rl::MovableObject
{
public:
	static rl::Object::Ptr create(const rl::Model& model)
	{
		return std::shared_ptr<Plane>(new Plane(model));
	}

	Vector6f getTorque() override;
	~Plane();

private:
	explicit Plane(const rl::Model& model);

private:
	const float m_wingArea;
};
