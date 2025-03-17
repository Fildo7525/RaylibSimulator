#pragma once

#include <cstdint>
#include <functional>
#include <raylib.h>
#include <string>
#include <vector>

#include "object.h"

namespace rl
{
class Application
{
public:
	struct Config
	{
		uint8_t fps;
		int monitor;
		std::function<void(const Application&)> onInit;
		std::function<void(const Application&)> onDeinit;
		float opacity;
		int screenHeight;
		int screenWidth;
		std::string windowTitle;
		std::pair<int, int> windowPosition;
		::Camera camera;
	};

	explicit Application(const Config& config);

	void addObject(const rl::Object::Ptr model);

	void run();

	~Application();

private:
	Config m_config;
	Camera m_camera;
	std::vector<rl::Object::Ptr> m_objects;
};



} // namespace rl
