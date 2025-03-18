#pragma once

#include <memory>
#include <raylib.h>
#include <string>
#include <map>

namespace rl
{

struct Model
{
	Model(const std::string &modelPath, const std::string &texturePath, const Vector3 &position = Vector3{ 0.0f, 0.0f, 0.0f }, float scale = 1.0f)
		: modelPath(modelPath)
		, texturePath(texturePath)
		, position(position)
		, scale(scale)
	{
	}

	std::string modelPath;
	std::string texturePath;
	Vector3 position;
	float scale;
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
