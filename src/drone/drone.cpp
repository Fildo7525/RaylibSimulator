#include "drone.h"
#include "quaternion.h"

#include <algorithm>
#include <execution>
#include <iostream>
#include <print>
#include <raylib.h>
#include <raymath.h>

Drone::Drone(const rl::Model& model)
	: rl::MovableObject(model)
{
	std::cout << "Model rotation: " << rl::Quaternion::fromEuler(model.rotation).toEuler() << std::endl;
}

Drone::~Drone()
{
}

Vector6f Drone::getTorque()
{
	Vector6f tau = Vector6f::Zero();

	if (IsKeyDown(KEY_C) && IsKeyDown(KEY_LEFT_SHIFT)) {
		std::println("Forcing the position {} and rotation {}", m_rlModel.spawnPosition, m_rlModel.rotation);
		forceStop(m_rlModel.spawnPosition, m_rlModel.rotation);
		return tau;
	}

	if (IsGamepadAvailable(0)) {
		tau = getTorqueGamepad();
	} else {
		tau = getTorqueKeyboard();
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


Vector6f Drone::getTorqueKeyboard()
{
	const float &dTau = m_rlModel.dThrust;
	const float &dM = m_rlModel.dMoment;
	Vector6f tau = Vector6f::Zero();

	// if (IsKeyDown(KEY_LEFT)) m_tau[0] += dTau;
	// else if (IsKeyDown(KEY_RIGHT)) m_tau[0] -= dTau;

	if (IsKeyDown(KEY_UP)) tau[2] += dTau;
	else if (IsKeyDown(KEY_DOWN)) tau[2] -= dTau;

	// if (IsKeyDown(KEY_UP)) tau[2] += dTau;
	// else if (IsKeyDown(KEY_DOWN)) tau[2] -= dTau;

	if (IsKeyDown(KEY_W)) tau[3] += dM;
	else if (IsKeyDown(KEY_S)) tau[3] -= dM;

	if (IsKeyDown(KEY_Q)) tau[4] += dM;
	else if (IsKeyDown(KEY_E)) tau[4] -= dM;

	if (IsKeyDown(KEY_D)) tau[5] -= dM;
	else if (IsKeyDown(KEY_A)) tau[5] += dM;

	if (IsKeyDown(KEY_MINUS))
		m_rlModel.scale -= 0.01f;
	if (IsKeyDown(KEY_EQUAL))
		m_rlModel.scale += 0.01f;

	return tau;
}

Vector6f Drone::getTorqueGamepad()
{
	std::array<float, 4> gamepadAxes = {
		GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X),
		GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y),
		GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X),
		GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y)
	};

	// std::println("BEFORE: Gamepad axis: [{}, {}, {}, {}]", gamepadAxes[0], gamepadAxes[1], gamepadAxes[2], gamepadAxes[3]);
	// for (auto &axis : gamepadAxes) {
	// 	if (std::abs(axis) < 0.1f) axis = 0;
	// }
	std::replace_if(std::execution::par_unseq, gamepadAxes.begin(), gamepadAxes.end(), [this](const float &f) { return std::abs(f) < 0.7; }, 0);
	std::println("AFTER: Gamepad axis: [{:0.4f}, {:0.4f}, {:0.4f}, {:0.4f}]", gamepadAxes[0], gamepadAxes[1], gamepadAxes[2], gamepadAxes[3]);

	Vector6f tau = Vector6f::Zero();
	// tau[0] = -gamepadAxes[0] * m_rlModel.dThrust;
	tau[2] = -gamepadAxes[GAMEPAD_AXIS_LEFT_Y] * m_rlModel.thrust.y;
	tau[3] = -gamepadAxes[GAMEPAD_AXIS_RIGHT_Y] * m_rlModel.moment.y;
	tau[4] = -gamepadAxes[GAMEPAD_AXIS_RIGHT_X] * m_rlModel.moment.y;
	tau[5] = -gamepadAxes[GAMEPAD_AXIS_LEFT_X] * m_rlModel.moment.y;

	// Damping when no input
	tau[0] *= 0.96f;

	return tau;
}
