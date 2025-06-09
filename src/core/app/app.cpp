#include "app.h"

#include <algorithm>
#include <execution>
#include <raylib.h>
#include <raymath.h>
#include <rcamera.h>
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

	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetWindowState(FLAG_VSYNC_HINT);
	SetWindowOpacity(m_config.opacity);
	SetWindowSize(m_config.screenWidth, m_config.screenHeight);

	UpdateCamera(&m_camera, CAMERA_THIRD_PERSON);

	for (auto object : m_objects) {
		object->loadModel();
	}

	while (!WindowShouldClose())
	{
		if (IsKeyDown(KEY_ESCAPE)) {
			CloseWindow();
			return;
		}

		if (IsKeyDown(KEY_ONE)) {
			m_camera.up = Vector3{ 1.0f, 0.0f, 0.0f };
		}
		else if (IsKeyDown(KEY_TWO)) {
			m_camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
		}
		else if (IsKeyDown(KEY_THREE)) {
			m_camera.up = Vector3{ 0.0f, 10.0f, 0.0f };
		}

		BeginDrawing();
			ClearBackground(RAYWHITE);

			m_camera.target = m_objects[0]->rlModel().position;
			auto cameraPos = m_objects[0]->rotation().toRotationMatrix() * Vector3f{ 0.0f, 50.0f, -120.0f };

			m_camera.position = Vector3Add(m_camera.target, Vector3{ cameraPos.x(), cameraPos.y(), cameraPos.z() });
			// m_camera.up = Vector3{ 0.0f, 1.0f, 0.0f };

			// CameraRoll(&m_camera, m_objects[0]->rotation().toEuler().y());
			// CameraPitch(&m_camera, m_objects[0]->rotation().toEuler().x(), true, true, false);
			UpdateCamera(&m_camera, CAMERA_THIRD_PERSON);

			BeginMode3D(m_camera);

			DrawGrid(100, 10.0f);

			float dt = GetFrameTime();
			std::for_each(std::execution::par, m_objects.begin(), m_objects.end(), [this, &dt](Object::Ptr object) {
				object->update(dt);
				object->draw();
			});

			EndMode3D();

			DrawRectangle(10, 10, 250, 113, Fade(SKYBLUE, 0.5));
			DrawRectangleLines( 10, 10, 250, 113, BLUE);

			const auto &p = m_objects[0]->rlModel().position;
			const auto &q = m_objects[0]->rotation().toEuler(true);
			DrawText(TextFormat("Position:\n %10.2f\n %10.2f\n %10.2f", p.x, p.y, p.z), 20, 20, 10, BLACK);
			DrawText(TextFormat("Rotation:\n %10.2f\n %10.2f\n %10.2f", q(0), q(1), q(2)), 20, 70, 10, BLACK);

		EndDrawing();
	}
}

Application::~Application()
{
	m_config.onDeinit(*this);

	CloseWindow();
}

}
