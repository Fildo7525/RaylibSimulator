#include "loader.h"

#include <filesystem>
#include <fstream>
#include <print>

#include <nlohmann/json.hpp>

using nlohmann::json;

static size_t idCounter = 0;

rl::Model rl::Model::fromFile(const rl::Path &configPath)
{
	rl::Model config;
	std::println("Loading configuration from: {}", configPath.string());

	std::ifstream file(configPath.string(), std::ifstream::in);
	if (!file.is_open()) {
		throw std::runtime_error("Configuration file [" + configPath.string() + "] not found");
	}

	json jsonConfig = json::parse(file);

	// Read default types
	config.modelPath = jsonConfig.value("modelPath", rl::Path("model.obj"));
	config.scale = jsonConfig.value("scale", 1.0f);
	config.mass = jsonConfig.value("mass", 1.0f);

	// Read default vectors
	std::array<float, 3> read = jsonConfig.value("position", std::array<float, 3>{ 0.0f, 0.0f, 0.0f });
	config.position = Vector3{ read[0], read[1], read[2] };

	read = jsonConfig.value("rotation", std::array<float, 3>{ 0.0f, 0.0f, 0.0f });
	config.rotation = Vector3{ read[0], read[1], read[2] };

	// Camera configuration
	read = jsonConfig.value("camera", json::object()).value("offset", std::array<float, 3>{ 0.0f, 0.0f, 0.0f });
	config.camera.offset = Vector3{ read[0], read[1], read[2] };
	read = jsonConfig.value("camera", json::object()).value("up", std::array<float, 3>{ 0.0f, 1.0f, 0.0f });
	config.camera.up = Vector3{ read[0], read[1], read[2] };
	config.camera.fovY = jsonConfig.value("camera", json::object()).value("fov", 30.f);

	// Read input parameters
	std::array<float, 2> input = jsonConfig.value("thrust", std::array<float, 2>{ -10.0f, 10.0f });
	config.thrust = Vector2{ input[0], input[1] };
	config.dThrust = jsonConfig.value("dThrust", 1.0f);
	input = jsonConfig.value("moment", std::array<float, 2>{ -10.0f, 10.0f });
	config.moment = Vector2{ input[0], input[1] };
	config.dMoment = jsonConfig.value("dMoment", 1.0f);

	// Read optional texture path
	if (jsonConfig.contains("texturePath")) {
		config.texturePath = jsonConfig.value("texturePath", "model.png");
	}

	std::println("Loaded configuration: modelPath={}\n texturePath={}\n position=({:.2f}, {:.2f}, {:.2f})\n "
		"rotation=({:.2f}, {:.2f}, {:.2f})\n scale={:.2f}\n mass={:.2f}\n camera.position=({:.2f}, {:.2f}, {:.2f})\n "
		"camera.rotation=({:.2f}, {:.2f}, {:.2f})\n camera.fovY={:.2f}\n thrust={:.2f}\n moment={:.2f}",
		config.modelPath, config.texturePath,
		config.position.x, config.position.y, config.position.z,
		config.rotation.x, config.rotation.y, config.rotation.z,
		config.scale, config.mass,
		config.camera.offset.x, config.camera.offset.y, config.camera.offset.z,
		config.camera.up.x, config.camera.up.y, config.camera.up.z,
		config.camera.fovY, config.dThrust, config.dMoment);
	return config;
}

rl::ImageLoader &rl::ImageLoader::instance()
{
	static ImageLoader instance;
	return instance;
}

std::shared_ptr<::Model> rl::ImageLoader::loadModel(const rl::Model &model)
{
	std::filesystem::path modelPath = model.modelPath;
	bool modelExists = std::filesystem::exists(modelPath);
	modelExists ?
		std::print("Model path exists: {}\n", modelPath.string()) :
		std::print("Model path does not exist: {}\n", modelPath.string());

	std::filesystem::path texturePath = model.texturePath;
	bool textureExists = std::filesystem::exists(texturePath);
	textureExists ?
		std::print("Texture path exists: {}\n", texturePath.string()) :
		std::print("Texture path does not exist: {}\n", texturePath.string());

	auto hasher = std::hash<std::string>();
	size_t hash = hasher(model.modelPath) ^ hasher(model.texturePath);

	auto it = m_images.find(hash);
	if (it != m_images.end()) {
		auto m = *it->second;
		hash ^= std::hash<size_t>()(idCounter++);
		m_images[hash] = std::make_shared<::Model>(m);
		return m_images[hash];
	}

	::Model m = LoadModel(model.modelPath.c_str());
	if (!IsModelValid(m)) {
		std::print("[Error]: Model is not valid: {}\n", model.modelPath);
		return nullptr;
	}

	std::print("Model materials count: {}\n", m.materialCount);
	if (texturePath.empty() || !textureExists) {
		for (int i = 0; i < m.materialCount; ++i) {
			m.materials[i].maps[MATERIAL_MAP_DIFFUSE].color = BLUE;
		}
		std::print("[Warning]: No texture path provided or texture does not exist: {}\n", texturePath.string());
	} else {
		std::print("Loading texture from path: {}\n", texturePath.string());
		Texture2D texture = LoadTexture(model.texturePath.c_str());  // Load model texture
		m.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;			// Set map diffuse texture
	}
	m_images[hash] = std::make_shared<::Model>(m);
	return m_images[hash];
}

void rl::ImageLoader::forceUnload(const rl::Model &model)
{
	auto hasher = std::hash<std::string>();
	size_t hash = hasher(model.modelPath) ^ hasher(model.texturePath);

	auto it = m_images.find(hash);
	if (it != m_images.end()) {
		UnloadTexture(it->second->materials[0].maps[MATERIAL_MAP_DIFFUSE].texture);
		UnloadModel(*it->second);
		m_images.erase(it);
	}
}

rl::ImageLoader::~ImageLoader()
{
	for (auto& [hash, model] : m_images) {
		UnloadTexture(model->materials[0].maps[MATERIAL_MAP_DIFFUSE].texture);
		UnloadModel(*model);
	}
}
