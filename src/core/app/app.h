#pragma once

#include <cstdint>
#include <functional>
#include <raylib.h>
#include <string>
#include <vector>

#include "object.h"

namespace rl
{
/**
 * @class Application
 * @brief Application class that manages the main application loop, configuration, and rendering of 3D objects.
 * This class encapsulates the application logic, including initialization, rendering, and cleanup.
 */
class Application
{
public:
	/**
	 * @class Config
	 * @brief Configuration structure for the Application class.
	 * This structure holds various settings for the application, such as frame rate, monitor selection,
	 * camera settings, and window properties.
	 */
	struct Config
	{
		// Frame rate for the application.
		uint8_t fps;
		// Monitor index to use for the application window.
		int monitor;
		// Callback functions for initialization and deinitialization.
		std::function<void(const Application&)> onInit;
		// Callback function for deinitialization.
		std::function<void(const Application&)> onDeinit;
		// Callback function for rendering.
		float opacity;
		// Screen height and width for the application window.
		int screenHeight;
		// Screen width for the application window.
		int screenWidth;
		// Title of the application window.
		std::string windowTitle;
		// Position of the application window on the screen.
		std::pair<int, int> windowPosition;
		// Camera to be used in the application.
		::Camera *camera = nullptr;
	};

	/**
	 * @brief Constructs an Application instance with the provided configuration.
	 *
	 * @param config Configuration settings for the application.
	 * @see rl::Application::Config
	 */
	explicit Application(const Config& config);

	/**
	 * @brief Adds an object to the application to be rendered
	 * This function needs to be called before running the application.
	 * It allows the application to manage and render multiple 3D objects.
	 *
	 * @param model Smart pointer to the object model to be added.
	 */
	void addObject(const rl::Object::Ptr model);

	/**
	 * @brief Runs the main application loop.
	 */
	void run();

	~Application();

private:
	Config m_config;
	Camera m_camera;
	std::vector<rl::Object::Ptr> m_objects;
};



} // namespace rl
