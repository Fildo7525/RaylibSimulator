#pragma once

#include <memory>

#include <raylib.h>
#include <raymath.h>

#include "loader.h"
#include "quaternion.h"

namespace rl
{

class Object
{
public:
	using Ptr = std::shared_ptr<Object>;

	Object(const rl::Model &model);
	virtual ~Object();

	void loadModel();

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
	rl::Model m_rlModel;
	std::shared_ptr<::Model> m_model;
};

}
