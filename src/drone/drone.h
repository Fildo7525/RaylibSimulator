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

protected:
	void forceStop() override
	{
		m_eta = Vector6f::Zero();
		Object::forceStop();
	}

private:
	Vector6f m_eta{ 0, 0, 0, 0, 0, 0 };
};
