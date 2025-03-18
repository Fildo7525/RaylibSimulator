#pragma once

#include "loader.h"
#include <memory>
#include <raylib.h>
#include <raymath.h>
#include <functional>

namespace rl
{

class Object
{
	friend class std::hash<rl::Object>;

public:
	using Ptr = std::shared_ptr<Object>;

	Object(const rl::Model &model);
	virtual ~Object();

	void loadModel();

	void draw() const;
	virtual void update(float dt) = 0;

	rl::Model rlModel() const;
	std::shared_ptr<::Model> model() const;

protected:
	rl::Model m_rlModel;
	std::shared_ptr<::Model> m_model;
};

}
