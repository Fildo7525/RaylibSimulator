#pragma once

#include <memory>

#include <raylib.h>
#include <raymath.h>

#include "loader.h"
#include "quaternion.h"

using Matrix6f = Eigen::Matrix<float, 6, 6>;
using Matrix3f = Eigen::Matrix<float, 3, 3>;

namespace rl
{

class Object
{
public:
	using Ptr = std::shared_ptr<Object>;

	Object(const rl::Model &model);
	virtual ~Object();

	void loadModel();

	Vector6f rigidBody(Vector6f &tau, float dt);
	rl::Quaternion rotation() const;

	void transform(const rl::Quaternion &quat);
	void move(const Eigen::Vector3f &position);

	void draw() const;
	virtual void update(float dt) = 0;

	rl::Model rlModel() const;
	std::shared_ptr<::Model> model() const;

	/* TODO:
	 * 1. Add collision check of the model and another object
	 **/

protected:
	virtual void forceStop();

protected:
	rl::Model m_rlModel;
	std::shared_ptr<::Model> m_model;
	Matrix6f m_invMrb;
	Matrix3f m_inertiaMatrix;
	Vector6f m_feedbackTau;
	Vector6f m_tau;
};

}
