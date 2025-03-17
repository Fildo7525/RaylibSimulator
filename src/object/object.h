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
	Object(const rl::Model &model);
	~Object();

	void loadModel();

	void draw() const;
	void update();

	rl::Model rlModel() const;
	std::shared_ptr<::Model> model() const;

private:
	rl::Model m_rlModel;
	std::shared_ptr<::Model> m_model;
};

}
