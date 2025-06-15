#include "app.h"

#include <algorithm>
#include <execution>
#include <raylib.h>
#include <raymath.h>
#include <rcamera.h>
#include <print>

constexpr Vector3 CAMERA_DEFAULT_POSITION{ 0.0f, 5.0f, -15.0f };

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
		m_camera.position = CAMERA_DEFAULT_POSITION;// Camera position perspective
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
	static uint8_t idx = 0;
	SetWindowMonitor(m_config.monitor);
	InitWindow(m_config.screenWidth, m_config.screenHeight, m_config.windowTitle.c_str());

	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetWindowState(FLAG_VSYNC_HINT);
	SetWindowOpacity(m_config.opacity);
	SetWindowSize(m_config.screenWidth, m_config.screenHeight);

	UpdateCamera(&m_camera, CAMERA_CUSTOM);

	for (auto object : m_objects) {
		object->loadModel();
	}

	std::println("Loaded {} objects", m_objects.size());
	auto rotate = [this](const Object::Ptr object, const Vector3 &rotation) {
		return object->rotation().rotate(rotation).toRlVector3();
	};

	while (!WindowShouldClose())
	{
		if (IsKeyDown(KEY_ESCAPE)) {
			CloseWindow();
			return;
		}

		if (IsKeyPressed(KEY_I)) {
			idx = (idx + 1) % m_objects.size();
			std::println("Current object index: {}", idx);
		}

		BeginDrawing();
			ClearBackground(RAYWHITE);

			m_camera.target = m_objects[idx]->rlModel().position + rotate(m_objects[idx], Vector3{0.0f, 1.0f, 0.0f});
			m_camera.position = m_camera.target + rotate(m_objects[idx], CAMERA_DEFAULT_POSITION);
			m_camera.up = rotate(m_objects[idx], Vector3{0.0f, 1.0f, 0.0f});

			BeginMode3D(m_camera);

			DrawGrid(100, 1.0f);

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
