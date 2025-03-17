#pragma once

#include <memory>
#include <raylib.h>
#include <string>
#include <map>

namespace rl
{

struct Model
{
	std::string modelPath;
	std::string texturePath;
};

class ImageLoader
{
public:
	static ImageLoader &instance();

	std::shared_ptr<::Model> loadModel(const rl::Model &model);

	~ImageLoader();

private:
	std::map<size_t, std::shared_ptr<::Model>> m_images;
};

}

bool operator==(const ::Model& lhs, const ::Model& rhs);
