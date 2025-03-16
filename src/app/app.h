#pragma once

#include <filesystem>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

class Model;

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
	};

	struct Model
	{
		std::string modelPath;
		std::string texturePath;
	};

	explicit Application(const Config& config);

	void loadModelPath(const std::filesystem::path& path);
	void loadModels(const std::vector<Model>& paths);
	void loadModel(const Model& path);

	void run();

	~Application();

private:
	const Config m_config;
	std::unordered_map<size_t, ::Model> m_models;
};
