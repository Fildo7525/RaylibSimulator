#include "plane.h"
#include "quaternion.h"

#include <algorithm>
#include <execution>
#include <iostream>
#include <raylib.h>
#include <raymath.h>

Plane::Plane(const rl::Model& model)
	: rl::MovableObject(model)
{
}

Plane::~Plane()
{
}

Vector6f Plane::getTorque()
{
	const float &dTau = m_rlModel.dThrust;
	const float &dM = m_rlModel.dMoment;
	Vector6f tau = Vector6f::Zero();

	if (IsKeyDown(KEY_LEFT)) tau[0] += dTau;
	else if (IsKeyDown(KEY_RIGHT)) tau[0] -= dTau;

	if (IsKeyDown(KEY_HOME)) tau[1] += dTau;
	else if (IsKeyDown(KEY_END)) tau[1] -= dTau;

	if (IsKeyDown(KEY_UP)) tau[2] += dTau;
	else if (IsKeyDown(KEY_DOWN)) tau[2] -= dTau;

	if (IsKeyDown(KEY_W)) tau[3] += dM;
	else if (IsKeyDown(KEY_S)) tau[3] -= dM;

	if (IsKeyDown(KEY_Q)) tau[4] += dM;
	else if (IsKeyDown(KEY_E)) tau[4] -= dM;

	if (IsKeyDown(KEY_A)) tau[5] -= dM;
	else if (IsKeyDown(KEY_D)) tau[5] += dM;

	// if (IsKeyDown(KEY_MINUS))
	// 	m_rlModel.scale -= 0.01f;
	// if (IsKeyDown(KEY_EQUAL))
	// 	m_rlModel.scale += 0.01f;

	if (IsKeyDown(KEY_C) && IsKeyDown(KEY_LEFT_SHIFT)) {
		forceStop();
	}

	for (int i = 0; i < tau.size(); ++i) {
		auto &t = tau[i];
		if (std::abs(t) < 0.01f) t = 0;
		else if (i < 3) {
			// t *= 0.99;
			t = std::clamp(t, m_rlModel.thrust.x, m_rlModel.thrust.y);
		} else {
			// t *= 0.96;
			t = std::clamp(t, m_rlModel.moment.x, m_rlModel.moment.y);
		}
	}

	return tau;
}

