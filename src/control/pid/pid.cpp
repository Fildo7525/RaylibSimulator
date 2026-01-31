#include "pid.h"

#include <algorithm>

PID::PID(float kp, float ki, float kd, AntiWindeupOptions &&antiWindeup)
	: m_kp(kp)
	, m_ki(ki)
	, m_kd(kd)
	, m_previousError(.0f)
	, m_integrated(.0f)
	, m_antiWindeup(std::move(antiWindeup))
{

}

float PID::update(float error, float dt, std::optional<float> derivative)
{
	if (not derivative.has_value()) {
		derivative = (error - m_previousError) / dt;
	}

	m_integrated += (error + m_antiWindeup.value) * dt;

	float u = m_kp * error + m_ki * m_integrated + m_kd * derivative.value_or(.0f);
	float u_saturated = saturate(u);
	float K_aw = 1.0/(m_ki * dt);

	m_antiWindeup.value = m_antiWindeup.enabled ? K_aw * (u_saturated - u) : 0;

	return u;
}

void PID::reset()
{
	m_integrated = 0;
	m_antiWindeup.value = 0;
	m_previousError = 0;
}

float PID::saturate(const float action_signal)
{
	return std::max(std::min(action_signal, m_antiWindeup.max_limit), m_antiWindeup.min_limit);
}
