#include "object.h"

#include <raymath.h>

rl::Object::Object(const rl::Model &model)
	: m_rlModel(model)
	, m_model(nullptr)
{
	std::println("Model path {}", m_rlModel.modelPath);
	std::println("Texture path {}", m_rlModel.texturePath);
}

rl::Object::~Object()
{
	rl::ImageLoader::instance().forceUnload(m_rlModel);
}

void rl::Object::loadModel()
{
	m_model = rl::ImageLoader::instance().loadModel(m_rlModel);
}

rl::Quaternion rl::Object::rotation() const
{
	return rl::Quaternion::fromRlRotMatrix(m_model->transform);
}

void rl::Object::draw() const
{
	// Draw 3d model with texture
	DrawModel(*m_model, m_rlModel.position, m_rlModel.scale, WHITE);
}

rl::Model rl::Object::rlModel() const
{
	return m_rlModel;
}

std::shared_ptr<::Model> rl::Object::model() const
{
	return m_model;
}
