#pragma once

#include "object.h"

using Matrix6f = Eigen::Matrix<float, 6, 6>;

namespace rl
{

class MovableObject
	: public Object
{
public:
	using Ptr = std::shared_ptr<MovableObject>;
	explicit MovableObject(const rl::Model &model);

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
	Matrix6f m_invMrb;
	Matrix3f m_inertiaMatrix;
	Vector6f m_feedbackTau;
	Vector6f m_tau;
	rl::Quaternion m_quat;
};

}
