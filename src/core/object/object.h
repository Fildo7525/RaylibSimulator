#pragma once

#include <memory>

#include <raylib.h>
#include <raymath.h>

#include "loader.h"
#include "quaternion.h"

using Matrix6f = Eigen::Matrix<float, 6, 6>;

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
	 * @brief Updates the object state based on the elapsed time.
	 *
	 * @param dt Elapsed time since the last update in seconds.
	 */
	void update(float dt);

	/**
	 * @brief Virtual method to get the torque applied to the object.
	 *
	 * @return Vector6f The torque vector applied to the object.
	 */
	virtual Vector6f getTorque() = 0;

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
	/**
	 * @brief Calculates the rigid body for the object.
	 *
	 * @param tau The torque vector applied to the object.
	 * @param dt Elapsed time since the last update in seconds.
	 * @return Vector6f The updated rigid body state after applying the torque.
	 */
	Vector6f rigidBody(Vector6f &tau, float dt);
	/**
	 * @brief Calculates the kinematics of the object based on the input control vector and time step.
	 *
	 * @param nu The control vector containing the input forces and torques.
	 * @param dt The time step for the kinematics calculation.
	 */
	std::pair<Eigen::Vector3f, rl::Quaternion> kinematics(const Vector6f &nu, float dt);

	/**
	 * @brief Transforms the object using the specified quaternion.
	 *
	 * @param quat The quaternion representing the rotation to be applied to the object.
	 */
	void transform(const rl::Quaternion &quat);
	/**
	 * @brief Moves the object to the specified position in 3D space.
	 *
	 * @param position The new position to move the object to.
	 */
	void move(const Eigen::Vector3f &position);


	/* TODO:
	 * 1. Add collision check of the model and another object
	 **/
	/**
	 * @brief Forces the object to stop immediately and reset its state.
	 */
	virtual void forceStop();

protected:
	rl::Model m_rlModel;
	std::shared_ptr<::Model> m_model;
	Matrix6f m_invMrb;
	Matrix3f m_inertiaMatrix;
	Vector6f m_feedbackTau;
	Vector6f m_tau;
	rl::Quaternion m_quat;
};

}
