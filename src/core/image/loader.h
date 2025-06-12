#pragma once

#include <filesystem>
#include <memory>
#include <raylib.h>
#include <string>
#include <map>

namespace rl
{

using Path = std::filesystem::path;

struct Model
{
	Model() = default;
	Model(const std::string &modelPath,
		  const std::string &texturePath,
		  const Vector3 &position = Vector3{ 0.0f, 0.0f, 0.0f },
		  const Vector3 &rotation = Vector3{ 0.0f, 0.0f, 0.0f },
		  float scale = 1.0f,
		  float mass = 1.0f)
		: modelPath(modelPath)
		, texturePath(texturePath)
		, position(position)
		, rotation(rotation)
		, scale(scale)
		, mass(mass)
	{
	}

	static Model fromFile(const rl::Path &config);

	std::string modelPath;
	std::string texturePath;
	Vector3 position;
	Vector3 rotation;
	float scale;
	float mass;
};

class ImageLoader
{
public:
	static ImageLoader &instance();

	std::shared_ptr<::Model> loadModel(const rl::Model &model);
	void forceUnload(const rl::Model &model);

	~ImageLoader();

private:
	std::map<size_t, std::shared_ptr<::Model>> m_images;
};

}
