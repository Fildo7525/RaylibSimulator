#include "app.h"

#include <raymath.h>
#include <print>

namespace rl
{

Application::Application(const Config& config)
	: m_config(config)
{
	SetTargetFPS(m_config.fps);

	// Use default values if missing initialization and deinitialization functions.
	if (m_config.onInit == nullptr) {
		m_config.onInit = [](const Application&) {};
	}
	if (m_config.onDeinit == nullptr) {
		m_config.onDeinit = [](const Application&) {};
	}

	// Use default values if opacity is invalid.
	if (m_config.opacity <= 0.0f || m_config.opacity > 1.0f) {
		m_config.opacity = 1.0f;
	}

	if (m_config.camera == nullptr) {
		m_camera.position = (Vector3){ 0.0f, 50.0f, -120.0f };// Camera position perspective
		m_camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };	  // Camera looking at point
		m_camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };		  // Camera up vector (rotation towards target)
		m_camera.fovy = 30.0f;								// Camera field-of-view Y
		m_camera.projection = CAMERA_PERSPECTIVE;			 // Camera type
	}
	else {
		m_camera = *m_config.camera;
	}

	m_config.onInit(*this);
}

void rl::Application::addObject(rl::Object::Ptr model)
{
	m_objects.push_back(model);
}

void Application::run()
{
	SetWindowMonitor(m_config.monitor);
	InitWindow(m_config.screenWidth, m_config.screenHeight, m_config.windowTitle.c_str());

	SetWindowOpacity(m_config.opacity);
	SetWindowSize(m_config.screenWidth, m_config.screenHeight);
	auto [x, y] = m_config.windowPosition;

	for (auto object : m_objects) {
		object->loadModel();
	}

	while (!WindowShouldClose())
	{
		BeginDrawing();
			ClearBackground(RAYWHITE);

			BeginMode3D(m_camera);

			for (const auto &object : m_objects) {
				object->draw();
			}

			EndMode3D();

		EndDrawing();
	}
}

Application::~Application()
{
	m_config.onDeinit(*this);

	CloseWindow();
}

}
