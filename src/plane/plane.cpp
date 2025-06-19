#include "plane.h"
#include "quaternion.h"

#include <algorithm>
#include <execution>
#include <iostream>
#include <raylib.h>
#include <raymath.h>

Plane::Plane(const rl::Model& model)
	: rl::Object(model)
{
}

Plane::~Plane()
{
}

Vector6f Plane::getTorque()
{
	const float &dTau = m_rlModel.dThrust;
	const float &dM = m_rlModel.dMoment;
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

	// if (IsKeyDown(KEY_MINUS))
	// 	m_rlModel.scale -= 0.01f;
	// if (IsKeyDown(KEY_EQUAL))
	// 	m_rlModel.scale += 0.01f;

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
			t *= 0.99;
			t = std::clamp(t, m_rlModel.thrust.x, m_rlModel.thrust.y);
		} else {
			t *= 0.96;
			t = std::clamp(t, m_rlModel.moment.x, m_rlModel.moment.y);
		}
	}

	return m_tau;
}

