#include "drone.h"
#include "quaternion.h"

#include <algorithm>
#include <execution>
#include <iostream>
#include <raylib.h>
#include <raymath.h>

Drone::Drone(const rl::Model& model)
	: rl::Object(model)
{
	std::cout << "Model rotation: " << rl::Quaternion::fromEuler(model.rotation).toEuler() << std::endl;
}

Drone::~Drone()
{
}

Vector6f Drone::getTorque()
{
	float dTau = 0.5;
	float dM = 12.5;
	// if (IsKeyDown(KEY_LEFT)) m_tau[0] += dTau;
	// else if (IsKeyDown(KEY_RIGHT)) m_tau[0] -= dTau;

	if (IsKeyDown(KEY_UP)) m_tau[1] += dTau;
	else if (IsKeyDown(KEY_DOWN)) m_tau[1] -= dTau;

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
		m_rlModel.rotation = Vector3{ 0, 0, 0 };
		m_tau = Vector6f::Zero();
		m_feedbackTau = Vector6f::Zero();
		m_rlModel.position = Vector3{0, 0, 0};
		m_eta = Vector6f::Zero();
	}

	for (int i = 0; i < m_tau.size(); ++i) {
		auto &t = m_tau[i];
		if (std::abs(t) < 0.01f) t = 0;
		else if (i < 3) {
			m_tau *= 0.99;
			t = std::clamp(t, -100.0f, 100.0f);
		} else {
			m_tau *= 0.96;
			t = std::clamp(t, -50.0f, 50.0f);
		}
	}

	return m_tau;
}

std::pair<Vector3f, rl::Quaternion> Drone::kinematics(const Vector6f &nu, float dt)
{
	// Euler angles
	float phi = m_eta[3];
	float theta = m_eta[4];
	float psi = m_eta[5];

	Matrix3f R = rl::Rx(phi) * rl::Ry(theta) * rl::Rz(psi);

	// Angular velocity transformation
	Matrix3f T{
		{1, std::sin(phi)*std::tan(theta), std::cos(phi)*std::tan(theta)},
		{0, std::cos(phi), -std::sin(phi)},
		{0, std::sin(phi)/std::cos(theta), std::cos(phi)/std::cos(theta)}
	};

	Matrix6f J = Matrix6f::Zero();
	J.block<3, 3>(0, 0) = R;
	J.block<3, 3>(3, 3) = T;

	auto eta_dot = J * nu;

	m_eta += eta_dot * dt;

	auto q = rl::Quaternion::fromEuler(m_eta.tail<3>());
	auto p = m_eta.head<3>();
	return { p, q };
}

void Drone::update(float dt)
{
	// auto m_tau = getTorque();
	/* std::cout << "Torque: " << m_tau << std::endl; */
	auto nu = rigidBody(m_tau, dt);
	auto [p, q] = kinematics(nu, dt);

	// Tranformation matrix for rotations
	transform(q);
	move(p);

	if (m_rlModel.position.y == 0 ) {
		forceStop();
	}
}
