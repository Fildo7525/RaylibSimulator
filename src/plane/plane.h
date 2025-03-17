#pragma once

#include "object.h"

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

	void update(double dt) override;

private:
	float m_pitch;
	float m_yaw;
	float m_roll;
};
