#pragma once

#include "object.h"
#include "quaternion.h"

class Drone
	: public rl::Object
{
public:
	static rl::Object::Ptr create(const rl::Model& model)
	{
		return std::make_shared<Drone>(model);
	}

	Drone(const rl::Model& model);
	~Drone();

	Vector6f getTorque();
	std::pair<Vector3f, rl::Quaternion> kinematics(const Vector6f &nu, float dt);

	void update(float dt) override;

private:
	rl::Quaternion m_quat;
};
