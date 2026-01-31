#pragma once

#include <optional>

struct AntiWindeupOptions
{
	bool enabled;
	float min_limit;
	float max_limit;
	float value;

	static AntiWindeupOptions createDefault()
	{
		return {.enabled = true, .min_limit = -1.0f, .max_limit = 1.0f, .value = 0.0f };
	}
};

class PID
{
public:

	PID(
		float Ki,
		float Kp,
		float Kd,
		AntiWindeupOptions &&antiWindeup = AntiWindeupOptions::createDefault()
	);

	float update(float error, float dt, std::optional<float> derivative = std::optional<float>());

	void reset();

private:
	float saturate(const float action_signal);

private:
	const float m_kp;
	const float m_ki;
	const float m_kd;

	float m_previousError;

	float m_integrated;
	AntiWindeupOptions m_antiWindeup;
};
