#include "plane.h"
#include "MovableObject.h"
#include "quaternion.h"

#include <algorithm>
#include <execution>
#include <iostream>
#include <raylib.h>
#include <raymath.h>

constexpr const float PLANE_WING_AREA = 1299.18f;
constexpr const float LIFT_COEFFICIENT = 0.8f;

Plane::Plane(const rl::Model& model)
	: rl::MovableObject(model)
	, m_wingArea(PLANE_WING_AREA * model.scale)
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

	const float lift = 0.5 * AIR_DENSITY_25CELSIUS * m_nu[2] * m_nu[2] * m_wingArea * LIFT_COEFFICIENT;
	// std::print("\rNu: ({}, {}, {})\n", m_nu.x(), m_nu.y(), m_nu.z());
	tau[1] += lift;
	std::println("Lift: {}", lift);
	std::println("Tau: ({}, {}, {})", tau[0], tau[1], tau[2]);
	std::println("");

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

