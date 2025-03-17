#include "object.h"

#include <print>

rl::Object::Object(const rl::Model &model)
	: m_rlModel(model)
	, m_model(nullptr)
{
	std::println("Model path {}", m_rlModel.modelPath);
	std::println("Texture path {}", m_rlModel.texturePath);
}

rl::Object::~Object()
{

}

void rl::Object::loadModel()
{
	m_model = rl::ImageLoader::instance().loadModel(m_rlModel);
}

void rl::Object::draw() const
{
	DrawModel(*m_model, m_rlModel.position, m_rlModel.scale, WHITE);   // Draw 3d model with texture
	DrawGrid(10, 10.0f);
}

rl::Model rl::Object::rlModel() const
{
	return m_rlModel;
}

std::shared_ptr<::Model> rl::Object::model() const
{
	return m_model;
}
