#include "object.h"

#include <raymath.h>

rl::Object::Object(const rl::Model &model)
	: m_rlModel(model)
	, m_model(nullptr)
	, m_invMrb(Matrix6f::Zero())
	, m_inertiaMatrix(Matrix3f::Zero())
	, m_feedbackTau(Vector6f::Zero())
{
	std::println("Model path {}", m_rlModel.modelPath);
	std::println("Texture path {}", m_rlModel.texturePath);

	m_inertiaMatrix.diagonal() << model.mass*2/6, model.mass*2/6, model.mass*2/6;

	m_invMrb.block<3, 3>(0, 0) = Eigen::Matrix3f::Identity() * model.mass;
	m_invMrb.block<3, 3>(3, 3) = m_inertiaMatrix;
	m_invMrb = m_invMrb.inverse();
}

rl::Object::~Object()
{
	rl::ImageLoader::instance().forceUnload(m_rlModel);
}

void rl::Object::loadModel()
{
	m_model = rl::ImageLoader::instance().loadModel(m_rlModel);
}

Vector6f rl::Object::rigidBody(Vector6f &tau, float dt)
{
	tau -= m_feedbackTau;

	Vector6f nu_dot = m_invMrb * tau;
	Vector6f nu = nu_dot * dt;

	Vector3f v = nu.head<3>();
	Vector3f omega = nu.tail<3>();

	auto tmp = m_inertiaMatrix * omega;

	auto pt1 = omega.cross(m_rlModel.mass * v);
	auto pt2 = -1 * tmp.cross(omega);
	m_feedbackTau.head<3>() = pt1;
	m_feedbackTau.tail<3>() = pt2;

	return nu;
}


rl::Quaternion rl::Object::rotation() const
{
	return rl::Quaternion::fromRlRotMatrix(m_model->transform);
}

void rl::Object::transform(const rl::Quaternion &quat)
{
	m_model->transform = quat.toRlRotMatrix();
}

void rl::Object::move(const Eigen::Vector3f &position)
{
	::Vector3 p = { position[0], position[1], position[2] };
	m_rlModel.position = Vector3Add(m_rlModel.position, p);
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
