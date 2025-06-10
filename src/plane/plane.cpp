#include "plane.h"
#include "quaternion.h"

#include <algorithm>
#include <execution>
#include <iostream>
#include <raylib.h>
#include <raymath.h>

Plane::Plane(const rl::Model& model)
	: rl::Object(model)
	, m_quat(rl::Quaternion::fromEuler(0, 0, 0))
	, m_mass(2.0f)
	, m_invMrb(Matrix6f::Zero())
	, m_inertiaMatrix(Matrix3f::Zero())
	, m_feedbackTau(Vector6f::Zero())
{
	m_inertiaMatrix.diagonal() << m_mass*2/6, m_mass*2/6, m_mass*2/6;

	m_invMrb.block<3, 3>(0, 0) = Eigen::Matrix3f::Identity() * m_mass;
	m_invMrb.block<3, 3>(3, 3) = m_inertiaMatrix;
	m_invMrb = m_invMrb.inverse();
}

Plane::~Plane()
{
}

Vector6f Plane::getTorque()
{
	float dTau = 300;
	float dM = 12.5;
	static Vector6f tau{ 0, 0, 0, 0, 0, 0 };

	if (IsKeyDown(KEY_LEFT)) tau[0] += dTau;
	else if (IsKeyDown(KEY_RIGHT)) tau[0] -= dTau;

	if (IsKeyDown(KEY_UP)) tau[2] += dTau;
	else if (IsKeyDown(KEY_DOWN)) tau[2] -= dTau;

	if (IsKeyDown(KEY_W)) tau[3] += dM;
	else if (IsKeyDown(KEY_S)) tau[3] -= dM;

	if (IsKeyDown(KEY_Q)) tau[4] += dM;
	else if (IsKeyDown(KEY_E)) tau[4] -= dM;

	if (IsKeyDown(KEY_A)) tau[5] -= dM;
	else if (IsKeyDown(KEY_D)) tau[5] += dM;

	if (IsKeyDown(KEY_MINUS))
		m_rlModel.scale -= 0.01f;
	if (IsKeyDown(KEY_EQUAL))
		m_rlModel.scale += 0.01f;

	if (IsKeyDown(KEY_C) && IsKeyDown(KEY_LEFT_SHIFT)) {
		m_quat = rl::Quaternion::fromEuler(0, 0, 0);
		tau = Vector6f::Zero();
		m_feedbackTau = Vector6f::Zero();
		m_rlModel.position = {0,0,0};
	}

	if (IsKeyDown(KEY_P)) {
		std::println("Current position:\n\tX: {}\n\tY: {}\n\tZ: {}", m_rlModel.position.x, m_rlModel.position.y, m_rlModel.position.z);
	}


	for (int i = 0; i < tau.size(); ++i) {
		auto &t = tau[i];
		if (std::abs(t) < 0.01f) t = 0;
		else if (i < 3) {
			tau *= 0.99;
			t = std::clamp(t, -10'000.0f, 10'000.0f);
		} else {
			tau *= 0.96;
			t = std::clamp(t, -10.0f, 10.0f);
		}
	}

	return tau;
}

Vector6f Plane::rigidBodyDynamics(Vector6f &tau, float dt)
{
	tau -= m_feedbackTau;

	Vector6f nu_dot = m_invMrb * tau;
	Vector6f nu = nu_dot * dt;

	Vector3f v = nu.head<3>();
	Vector3f omega = nu.tail<3>();

	auto tmp = m_inertiaMatrix * omega;

	auto pt1 = omega.cross(m_mass * v);
	auto pt2 = -1 * tmp.cross(omega);
	m_feedbackTau.head<3>() = pt1;
	m_feedbackTau.tail<3>() = pt2;

	// for (auto& t : m_feedbackTau) {
	// 	t = std::clamp(t, -1000.0f, 1000.0f);
	// 	t = std::abs(t) < 0.1f ? 0 : t;
	// }

	return nu;
}

std::pair<Eigen::Vector3f, rl::Quaternion> Plane::kinematics(const Vector6f &nu, float dt)
{
	constexpr int L = 100;
	Eigen::Vector3f p;
	rl::Quaternion tau_bar(nu[0], nu[1], nu[2], 0);
	rl::Quaternion omega_bar(nu[3], nu[4], nu[5], 0);
	rl::Quaternion q = m_quat;

	// Position
	rl::Quaternion p_dot = q * (tau_bar * q.cconjugate());
	p = p_dot.data().head<3>() * dt;

	// Rotation
	rl::Quaternion q_dot = 0.5 * (m_quat * omega_bar);
	m_quat = m_quat + q_dot * dt;

	/* std::println("transpose: {}, {}, {}, {}", transpose[0], transpose[1], transpose[2], transpose[3]); */
	m_quat += L * (1 - m_quat.ctranspose().dot(m_quat)) * m_quat;
	m_quat.normalize();

	return { p, m_quat };
}

void Plane::update(float dt)
{
	auto tau = getTorque();
	/* std::cout << "Torque: " << tau << std::endl; */
	auto nu = rigidBodyDynamics(tau, dt);
	auto [p, q] = kinematics(nu, dt);

	// Tranformation matrix for rotations
	transform(q);
	move(p);
}
