#pragma once

#include "object.h"
#include "quaternion.h"

class Plane
	: public rl::Object
{
	using Matrix6f = Eigen::Matrix<float, 6, 6>;
	using Matrix3f = Eigen::Matrix<float, 3, 3>;
public:
	static rl::Object::Ptr create(const rl::Model& model)
	{
		return std::make_shared<Plane>(model);
	}

	Plane(const rl::Model& model);
	~Plane();

	Vector6f getTorque();
	Vector6f rigidBodyDynamics(Vector6f &tau, float dt);
	std::pair<Eigen::Vector3f, rl::Quaternion> kinematics(const Vector6f &nu, float dt);

	void update(float dt) override;

private:
	rl::Quaternion m_quat;
	Matrix6f m_invMrb;
	Matrix3f m_inertiaMatrix;
	Vector6f m_feedbackTau;
};
