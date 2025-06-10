#include "loader.h"
#include <filesystem>
#include <print>

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
	if (it != m_images.end())
		return it->second;

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
