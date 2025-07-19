#pragma once

#include <memory>

#include <raylib.h>
#include <raymath.h>

#include "loader.h"
#include "quaternion.h"

namespace rl
{

/**
 * @brief Creates a rotation matrix around the X-axis.
 *
 * @param angle Angle in radians to rotate around the X-axis.
 * @return Matrix3f Rotation matrix for the X-axis rotation.
 */
inline Matrix3f Rx(float angle)
{
	return Matrix3f{
		{1, 0, 0},
		{0, std::cos(angle), -std::sin(angle)},
		{0, std::sin(angle), std::cos(angle)}
	};
}

/**
 * @brief Creates a rotation matrix around the Y-axis.
 *
 * @param angle Angle in radians to rotate around the Y-axis.
 * @return Matrix3f Rotation matrix for the Y-axis rotation.
 */
inline Matrix3f Ry(float angle)
{
	return Matrix3f{
		{std::cos(angle), 0, std::sin(angle)},
		{0, 1, 0},
		{-std::sin(angle), 0, std::cos(angle)}
	};
}

/**
 * @brief Creates a rotation matrix around the Z-axis.
 *
 * @param angle Angle in radians to rotate around the Z-axis.
 * @return Matrix3f Rotation matrix for the Z-axis rotation.
 */
inline Matrix3f Rz(float angle)
{
	return Matrix3f{
		{std::cos(angle), -std::sin(angle), 0},
		{std::sin(angle), std::cos(angle), 0},
		{0, 0, 1}
	};
}

/**
 * @class Object
 * @brief Base class for 3D objects in the simulation.
 *
 * This class represents a 3D object with a model, physics properties, and methods for
 * loading, updating, and rendering the object. The object physics are based on rigid body dynamics,
 * and it provides methods to calculate torques and kinematics. The rotations are handled using quaternions,
 * and the object can be transformed and moved in 3D space.
 */
class Object
{
public:
	using Ptr = std::shared_ptr<Object>;

	/**
	 * @brief Constructs an Object with the specified model.
	 *
	 * @param model The model to be used for the object.
	 */
	Object(const rl::Model &model);
	virtual ~Object();

	/**
	 * @brief Loads the model for the object.
	 */
	void loadModel();

	/**
	 * @brief Returns the current rotation of the object represented as a quaternion.
	 */
	rl::Quaternion rotation() const;

	/**
	 * @brief Draws the object in the 3D space.
	 */
	void draw() const;

	/**
	 * @brief Returns the internal model representation of the object.
	 */
	rl::Model rlModel() const;
	/**
	 * @brief Returns the raylib model associated with this object.
	 */
	std::shared_ptr<::Model> model() const;


protected:
	rl::Model m_rlModel;
	std::shared_ptr<::Model> m_model;
};

}
