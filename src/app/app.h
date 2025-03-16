#pragma once

#include <filesystem>
#include <functional>
#include <map>
#include <raylib.h>
#include <string>
#include <unordered_set>
#include <vector>

namespace std
{
	template<>
	struct hash<::Model>
	{
		std::size_t operator()(const ::Model& model) const
		{
			int hash = model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture.width ^
				model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture.height ^
				model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture.mipmaps ^
				model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture.format ^
				model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture.id;
			return std::hash<int>{}(hash);
		}
	};
}

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
		std::map<std::string, std::string> models;
	};

	struct Model
	{
		std::string modelPath;
		std::string texturePath;
	};

	explicit Application(const Config& config);
	void run();

	~Application();


private:

	void loadModelPath(const std::filesystem::path& path);
	void loadModels(const std::vector<Model>& paths);
	void loadModel(const Model& path);

private:
	Config m_config;
	Camera m_camera;
	std::unordered_set<::Model> m_models;
};


bool operator==(const ::Model& lhs, const ::Model& rhs);
bool operator==(const Application::Model& lhs, const Application::Model& rhs);
