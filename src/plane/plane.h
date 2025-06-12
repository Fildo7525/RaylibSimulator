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

	Vector6f getTorque();
	std::pair<Eigen::Vector3f, rl::Quaternion> kinematics(const Vector6f &nu, float dt);

	void update(float dt) override;

private:
	rl::Quaternion m_quat;
};
