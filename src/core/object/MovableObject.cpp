#include "MovableObject.h"

#include <raymath.h>

rl::MovableObject::MovableObject(const rl::Model &model)
	: rl::Object(model)
	, m_invMrb(Matrix6f::Zero())
	, m_inertiaMatrix(Matrix3f::Zero())
	, m_feedbackTau(Vector6f::Zero())
	, m_tau(Vector6f::Zero())
	, m_quat(rl::Quaternion::fromEuler(model.rotation))
{
	m_inertiaMatrix = m_rlModel.inertia;

	m_invMrb.block<3, 3>(0, 0) = Eigen::Matrix3f::Identity() * model.mass;
	m_invMrb.block<3, 3>(3, 3) = m_inertiaMatrix;
	m_invMrb = m_invMrb.inverse();
}

void rl::MovableObject::update(float dt)
{
	auto tau = getTorque();
	/* std::cout << "Torque: " << tau << std::endl; */
	auto nu = rigidBody(tau, dt);
	auto [p, q] = kinematics(nu, dt);

	// Tranformation matrix for rotations
	transform(q);
	move(p);
}

Vector6f rl::MovableObject::rigidBody(Vector6f &tau, float dt)
{
	tau -= m_feedbackTau;

	Vector6f nu_dot = m_invMrb * tau;
	Vector6f nu = nu_dot * dt;

	Vector3f v = nu.head<3>();
	Vector3f omega = nu.tail<3>();

	auto tmp = m_inertiaMatrix * omega;

	auto pt1 = omega.cross(m_rlModel.mass * v);
	auto pt2 = -1 * tmp.cross(omega);
	m_feedbackTau.head<3>() = pt1;
	m_feedbackTau.tail<3>() = pt2;

	return nu;
}

std::pair<Eigen::Vector3f, rl::Quaternion> rl::MovableObject::kinematics(const Vector6f &nu, float dt)
{
	constexpr int L = 100;
	Vector3f p;
	rl::Quaternion omega_bar(nu[3], nu[4], nu[5], 0);

	// Position
	rl::Quaternion p_dot = m_quat.rotate(nu.head<3>());
	p = p_dot.data().head<3>() * dt;

	// Rotation
	rl::Quaternion q_dot = 0.5 * (m_quat * omega_bar);
	m_quat = m_quat + q_dot * dt;

	/* std::println("transpose: {}, {}, {}, {}", transpose[0], transpose[1], transpose[2], transpose[3]); */
	m_quat += L * (1 - m_quat.ctranspose().dot(m_quat)) * m_quat;
	m_quat.normalize();

	return { p, m_quat };
}

void rl::MovableObject::transform(const rl::Quaternion &quat)
{
	m_model->transform = quat.toRlRotMatrix();
}

void rl::MovableObject::move(const Eigen::Vector3f &position)
{
	::Vector3 p = { position[0], position[1], position[2] };
	m_rlModel.position = Vector3Add(m_rlModel.position, p);
}

void rl::MovableObject::forceStop()
{
	m_rlModel.position.y = 0.0f;
	m_feedbackTau = Vector6f::Zero();
	m_tau = Vector6f::Zero();
}
