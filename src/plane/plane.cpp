#include "plane.h"
#include "quaternion.h"

Plane::Plane(const rl::Model& model)
	: rl::Object(model)
	, m_pitch(0.0f)
	, m_yaw(0.0f)
	, m_roll(0.0f)
{
}

Plane::~Plane()
{
}

void Plane::update()
{
	if (IsKeyDown(KEY_DOWN)) m_pitch += 0.6f;
	else if (IsKeyDown(KEY_UP)) m_pitch -= 0.6f;
	else
	{
		if (m_pitch > 0.3f) m_pitch -= 0.3f;
		else if (m_pitch < -0.3f) m_pitch += 0.3f;
	}

	// Plane m_yaw (y-axis) controls
	if (IsKeyDown(KEY_S)) m_yaw -= 1.0f;
	else if (IsKeyDown(KEY_A)) m_yaw += 1.0f;
	else
	{
		if (m_yaw > 0.0f) m_yaw -= 0.5f;
		else if (m_yaw < 0.0f) m_yaw += 0.5f;
	}

	// Plane m_roll (z-axis) controls
	if (IsKeyDown(KEY_LEFT)) m_roll -= 1.0f;
	else if (IsKeyDown(KEY_RIGHT)) m_roll += 1.0f;
	else
	{
		if (m_roll > 0.0f) m_roll -= 0.5f;
		else if (m_roll < 0.0f) m_roll += 0.5f;
	}

	// Tranformation matrix for rotations
	m_model->transform = MatrixRotateXYZ((Vector3){ DEG2RAD*m_pitch, DEG2RAD*m_yaw, DEG2RAD*m_roll });
}
