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
	std::filesystem::exists(modelPath) ?
		std::print("Model path exists: {}\n", modelPath.string()) :
		std::print("Model path does not exist: {}\n", modelPath.string());

	std::filesystem::path texturePath = model.texturePath;
	std::filesystem::exists(texturePath) ?
		std::print("Texture path exists: {}\n", texturePath.string()) :
		std::print("Texture path does not exist: {}\n", texturePath.string());

	auto hasher = std::hash<std::string>();
	size_t hash = hasher(model.modelPath) ^ hasher(model.texturePath);

	auto it = m_images.find(hash);
	if (it != m_images.end())
		return it->second;

	::Model m = LoadModel(model.modelPath.c_str());				  // Load model
	Texture2D texture = LoadTexture(model.texturePath.c_str());  // Load model texture
	m.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;			// Set map diffuse texture
	m_images[hash] = std::make_shared<::Model>(m);
	return m_images[hash];
}

bool operator==(const ::Model& lhs, const ::Model& rhs)
{
	return lhs.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture.id == rhs.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture.id &&
		lhs.boneCount == rhs.boneCount &&
		lhs.meshCount == rhs.meshCount &&
		lhs.materialCount == rhs.materialCount;

}

rl::ImageLoader::~ImageLoader()
{
	for (auto& [hash, model] : m_images) {
		UnloadTexture(model->materials[0].maps[MATERIAL_MAP_DIFFUSE].texture);
		UnloadModel(*model);
	}
}
