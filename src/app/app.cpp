#include "app.h"

#include <raylib.h>

Application::Application(const Config& config)
	: m_config(config)
{
	SetTargetFPS(m_config.fps);

	if (m_config.monitor >= 0 && m_config.monitor < GetMonitorCount())
		SetWindowMonitor(m_config.monitor);
	else
		SetWindowMonitor(-1);

	SetWindowOpacity(m_config.opacity);
	SetWindowSize(m_config.screenWidth, m_config.screenHeight);

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

	auto hash = std::hash<std::string>()(model.modelPath);
	m_models[hash] = m;
}

void Application::run()
{
	InitWindow(m_config.screenWidth, m_config.screenHeight, m_config.windowTitle.c_str());

	while (!WindowShouldClose())
	{
		BeginDrawing();
			ClearBackground(RAYWHITE);
			DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
		EndDrawing();
	}
}

Application::~Application()
{
	m_config.onDeinit(*this);

	CloseWindow();
}
