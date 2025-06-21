#pragma once

#include <filesystem>
#include <memory>
#include <raylib.h>
#include <string>
#include <map>
#include <Eigen/Dense>

namespace rl
{

using Matrix3f = Eigen::Matrix<float, 3, 3>;
using Path = std::filesystem::path;

/**
 * @class Model
 * @brief Represents a 3D model with associated properties such as position, rotation, scale, mass, and camera settings.
 *
 * These settings are used to define the model's behavior in a 3D space, including its physics properties and camera
 * perspective. The settings can be loaded from a configuration file, allowing for flexible model definitions.
 */
struct Model
{
	struct Camera {
		Vector3 offset;
		Vector3 up;
		float fovY;
	};

	Model() = default;
	Model(const std::string &modelPath,
		  const std::string &texturePath,
		  float thrust,
		  float dThrust,
		  float dMoment,
		  float moment,
		  const Vector3 &position = Vector3{ 0.0f, 0.0f, 0.0f },
		  const Vector3 &rotation = Vector3{ 0.0f, 0.0f, 0.0f },
		  float scale = 1.0f,
		  float mass = 1.0f,
		  const Camera &camera = Camera{ Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 0.0f, 1.0f, 0.0f }, 30.0f })
		: modelPath(modelPath)
		, texturePath(texturePath)
		, position(position)
		, rotation(rotation)
		, scale(scale)
		, mass(mass)
		, camera(camera)
		, thrust(thrust)
		, moment(moment)
		, dThrust(dThrust)
		, dMoment(dMoment)
	{
	}

	/**
	 * @brief Loads a model configuration from a file.
	 *
	 * @param config std::filesystem::path to the configuration file containing model settings.
	 * @return Model A Model object initialized with the settings from the file.
	 */
	static Model fromFile(const rl::Path &config);

	std::string modelPath;
	std::string texturePath;
	Vector3 position;
	Vector3 rotation;
	float scale;
	float mass;
	Camera camera;
	float dThrust;
	Vector2 thrust; // Min and max thrust
	float dMoment;
	Vector2 moment; // Min and max thrust
	Matrix3f inertia;
};

/**
 * @class ImageLoader
 * @brief Singleton class for loading and managing 3D models and their textures.
 */
class ImageLoader
{
public:
	/**
	 * @brief Returns the singleton instance of ImageLoader.
	 *
	 * @return ImageLoader& Reference to the singleton instance.
	 */
	static ImageLoader &instance();

	/**
	 * @brief Loads a 3D model from the given rl::Model configuration.
	 *
	 * @param model The rl::Model configuration containing model properties such as path, texture, position, rotation, scale,
	 * and mass.
	 */
	std::shared_ptr<::Model> loadModel(const rl::Model &model);
	/**
	 * @brief Unloads a 3D model from the loader.
	 *
	 * @param model The rl::Model configuration of the model to be unloaded.
	 */
	void forceUnload(const rl::Model &model);

	~ImageLoader();

private:
	ImageLoader() = default;
	ImageLoader(const ImageLoader &) = delete;
	ImageLoader &operator=(const ImageLoader &) = delete;

private:
	std::map<size_t, std::shared_ptr<::Model>> m_images;
};

}
