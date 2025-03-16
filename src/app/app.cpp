#include "app.h"

#include <raymath.h>

Application::Application(const Config& config)
	: m_config(config)
	, m_camera({ 0 })
	, m_models(1)
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

	m_camera.position = (Vector3){ 0.0f, 50.0f, -120.0f };// Camera position perspective
	m_camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };	  // Camera looking at point
	m_camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };		  // Camera up vector (rotation towards target)
	m_camera.fovy = 30.0f;								// Camera field-of-view Y
	m_camera.projection = CAMERA_PERSPECTIVE;			 // Camera type

	m_config.onInit(*this);
}

void Application::loadModelPath(const std::filesystem::path& path)
{
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (!entry.is_regular_file()) {
			continue;
		}

		auto modelPath = entry.path();
		auto texturePath = modelPath;
		if (texturePath.extension() == ".png")
			loadModel({ modelPath.string(), texturePath.string() });
	}
}

void Application::loadModels(const std::vector<Application::Model>& models)
{
	for (const auto& model : models) {
		loadModel(model);
	}
}

void Application::loadModel(const Application::Model& model)
{
	::Model m = LoadModel(model.modelPath.c_str());
	if (!model.texturePath.empty()) {
		Texture2D texture = LoadTexture(model.texturePath.c_str());
		// Set map diffuse texture
		m.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
	}

	m_models.insert(m);
}

void Application::run()
{
	SetWindowMonitor(m_config.monitor);
	InitWindow(m_config.screenWidth, m_config.screenHeight, m_config.windowTitle.c_str());

	for (const auto& [modelPath, modelTexture] : m_config.models) {
		loadModel({modelPath, modelTexture});
	}

	SetWindowOpacity(m_config.opacity);
	SetWindowSize(m_config.screenWidth, m_config.screenHeight);
	auto [x, y] = m_config.windowPosition;

	while (!WindowShouldClose())
	{
		BeginDrawing();
			ClearBackground(RAYWHITE);

			BeginMode3D(m_camera);

			for (const auto &model : m_models) {
				DrawModel(model, (Vector3){ 0.0f, -8.0f, 0.0f }, 1.0f, WHITE);   // Draw 3d model with texture
				DrawGrid(10, 10.0f);
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

bool operator==(const ::Model& lhs, const ::Model& rhs)
{
	return lhs.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture.width == rhs.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture.width &&
		lhs.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture.height == rhs.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture.height &&
		lhs.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture.mipmaps == rhs.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture.mipmaps &&
		lhs.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture.format == rhs.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture.format &&
		lhs.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture.id == rhs.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture.id;
}

bool operator==(const Application::Model& lhs, const Application::Model& rhs)
{
	return lhs.modelPath == rhs.modelPath && lhs.texturePath == rhs.texturePath;
}
