#include "plane.h"
#include "quaternion.h"

#include <algorithm>
#include <execution>
#include <iostream>
#include <raylib.h>
#include <raymath.h>

Plane::Plane(const rl::Model& model)
	: rl::Object(model)
	, m_quat(rl::Quaternion::fromEuler(model.rotation))
{
	std::cout << "Model rotation: " << rl::Quaternion::fromEuler(model.rotation).toEuler() << std::endl;
}

Plane::~Plane()
{
}

Vector6f Plane::getTorque()
{
	float dTau = 300;
	float dM = 12.5;
	static Vector6f m_tau{ 0, 0, 0, 0, 0, 0 };

	if (IsKeyDown(KEY_LEFT)) m_tau[0] += dTau;
	else if (IsKeyDown(KEY_RIGHT)) m_tau[0] -= dTau;

	if (IsKeyDown(KEY_UP)) m_tau[2] += dTau;
	else if (IsKeyDown(KEY_DOWN)) m_tau[2] -= dTau;

	if (IsKeyDown(KEY_W)) m_tau[3] += dM;
	else if (IsKeyDown(KEY_S)) m_tau[3] -= dM;

	if (IsKeyDown(KEY_Q)) m_tau[4] += dM;
	else if (IsKeyDown(KEY_E)) m_tau[4] -= dM;

	if (IsKeyDown(KEY_A)) m_tau[5] -= dM;
	else if (IsKeyDown(KEY_D)) m_tau[5] += dM;

	if (IsKeyDown(KEY_MINUS))
		m_rlModel.scale -= 0.01f;
	if (IsKeyDown(KEY_EQUAL))
		m_rlModel.scale += 0.01f;

	if (IsKeyDown(KEY_C) && IsKeyDown(KEY_LEFT_SHIFT)) {
		m_quat = rl::Quaternion::fromEuler(m_rlModel.rotation);
		m_tau = Vector6f::Zero();
		m_feedbackTau = Vector6f::Zero();
		m_rlModel.position = Vector3{0, 0, 0};
	}

	for (int i = 0; i < m_tau.size(); ++i) {
		auto &t = m_tau[i];
		if (std::abs(t) < 0.01f) t = 0;
		else if (i < 3) {
			m_tau *= 0.99;
			t = std::clamp(t, -10'000.0f, 10'000.0f);
		} else {
			m_tau *= 0.96;
			t = std::clamp(t, -1000.0f, 1000.0f);
		}
	}

	return m_tau;
}

std::pair<Eigen::Vector3f, rl::Quaternion> Plane::kinematics(const Vector6f &nu, float dt)
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

void Plane::update(float dt)
{
	auto tau = getTorque();
	/* std::cout << "Torque: " << tau << std::endl; */
	auto nu = rigidBody(tau, dt);
	auto [p, q] = kinematics(nu, dt);

	// Tranformation matrix for rotations
	transform(q);
	move(p);
}
