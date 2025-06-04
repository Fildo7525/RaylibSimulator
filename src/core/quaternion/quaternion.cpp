#include "quaternion.h"
#include <raymath.h>

Vector4f rl::Quaternion::toEigVector(const ::Quaternion &quat) const
{
	return Vector4f(quat.x, quat.y, quat.z, quat.w);
}

rl::Quaternion::Quaternion(float x, float y, float z, float w)
	: m_data{ x, y, z, w }
{
}

rl::Quaternion rl::Quaternion::fromEuler(float x, float y, float z)
{
	auto data = QuaternionFromEuler(y, z, x);
	return Quaternion(data.x, data.y, data.z, data.w);
}

Vector3f rl::Quaternion::toEuler(bool degrees) const
{
	::Quaternion quat{m_data.x(), m_data.y(), m_data.z(), m_data.w()};
	Vector3 angles = QuaternionToEuler(quat);
	Vector3f euler = { angles.y, angles.z, angles.x };
	euler *= (degrees ? 180.0f / PI : 1.0f);
	return euler;
}

rl::Quaternion rl::Quaternion::fromEigRotMatrix(Matrix4f matrix)
{
	::Matrix mat = { matrix(0, 0), matrix(0, 1), matrix(0, 2), matrix(0, 3),
					 matrix(1, 0), matrix(1, 1), matrix(1, 2), matrix(1, 3),
					 matrix(2, 0), matrix(2, 1), matrix(2, 2), matrix(2, 3),
					 matrix(3, 0), matrix(3, 1), matrix(3, 2), matrix(3, 3) };

	auto data = QuaternionFromMatrix(mat);
	return Quaternion(data.x, data.y, data.z, data.w);;
}

rl::Quaternion rl::Quaternion::fromRlRotMatrix(::Matrix matrix)
{
	auto data = QuaternionFromMatrix(matrix);
	return Quaternion(data.x, data.y, data.z, data.w);
}

::Quaternion rl::Quaternion::toRlQuaternion() const
{
	return ::Quaternion{ m_data.x(), m_data.y(), m_data.z(), m_data.w() };
}

::Matrix rl::Quaternion::toRlRotMatrix() const
{
	::Quaternion quat{ m_data.x(), m_data.y(), m_data.z(), m_data.w() };
	return QuaternionToMatrix(quat);
}

Matrix4f rl::Quaternion::toEigRotMatrix() const
{
	::Quaternion quat{ .x = m_data[0], .y = m_data[1], .z = m_data[2], .w = m_data[3] };
	::Matrix mat = QuaternionToMatrix(quat);
	Matrix4f matrix{{mat.m0, mat.m4, mat.m8, mat.m12},
					{mat.m1, mat.m5, mat.m9, mat.m13},
					{mat.m2, mat.m6, mat.m10, mat.m14},
					{mat.m3, mat.m7, mat.m11, mat.m15}};
	return matrix;
}

Vector4f rl::Quaternion::toEigVector() const
{
	return Vector4f(m_data.x(), m_data.y(), m_data.z(), m_data.w());
}

Vector4f rl::Quaternion::data() const
{
	return m_data;
}

float rl::Quaternion::magnitude() const
{
	return sqrt(m_data[0] * m_data[0] + m_data[1] * m_data[1] + m_data[2] * m_data[2] + m_data[3] * m_data[3]);
}

rl::Quaternion rl::Quaternion::cconjugate() const
{
	return Quaternion(-m_data[0], -m_data[1], -m_data[2], m_data[3]);
}

rl::Quaternion rl::Quaternion::cnormalize() const
{
	auto q_data = m_data;

	/* quat = quat + L*(1-self.quaternion.T @ self.quaternion) * self.quaternion */
	float mag = magnitude();
	mag = (mag == 0) ? 1 : mag;
	float gain = 1.0 / mag;

	q_data *= gain;

	return Quaternion(q_data[0], q_data[1], q_data[2], q_data[3]);
}

rl::Quaternion rl::Quaternion::crotate(const Quaternion& q) const
{
	auto conj = *this;
	conj = conj.conjugate();
	conj.m_data = toEigVector(QuaternionMultiply(toRlQuaternion(), conj.toRlQuaternion()));
	conj.m_data = toEigVector(QuaternionMultiply(q.toRlQuaternion(), conj.toRlQuaternion()));

	return conj;
}

rl::Quaternion rl::Quaternion::ctranspose() const
{
	Quaternion conj = *this;
	conj.m_data.transpose();

	return conj;
}

float rl::Quaternion::dot(const rl::Quaternion &q) const
{
	return m_data.dot(q.m_data);;
}

rl::Quaternion &rl::Quaternion::conjugate()
{
	m_data = { -m_data.x(), -m_data.y(), -m_data.z(), m_data.w() };
	return *this;
}

rl::Quaternion &rl::Quaternion::normalize()
{
	double mag = magnitude();
	mag = (mag == 0) ? 1 : mag;

	double gain = 1.0 / mag;
	this->m_data *= gain;

	return *this;
}

rl::Quaternion &rl::Quaternion::rotate(const Quaternion& q)
{
	auto conj = *this;
	conj *= q.cconjugate();

	conj.m_data = toEigVector(QuaternionMultiply(q.toRlQuaternion(), conj.toRlQuaternion()));
	m_data = toEigVector(QuaternionMultiply(toRlQuaternion(), conj.toRlQuaternion()));

	return *this;
}

rl::Quaternion rl::operator+(const rl::Quaternion& lhs, const rl::Quaternion& rhs)
{
	Quaternion q(0, 0, 0, 1);
	q.m_data = lhs.m_data + rhs.m_data;

	return q;
}

rl::Quaternion rl::operator-(const rl::Quaternion& lhs, const rl::Quaternion& rhs)
{
	Quaternion q(0, 0, 0, 1);
	q.m_data = lhs.m_data - rhs.m_data;

	return q;
}

rl::Quaternion rl::operator*(const rl::Quaternion& lhs, const rl::Quaternion& rhs)
{
	auto data = QuaternionMultiply(lhs.toRlQuaternion(), rhs.toRlQuaternion());
	return rl::Quaternion(data.x, data.y, data.z, data.w);
}

rl::Quaternion rl::operator*(const rl::Quaternion& lhs, double rhs)
{
	rl::Quaternion q(0, 0, 0, 1);
	q.m_data = lhs.m_data * rhs;

	return q;
}

rl::Quaternion rl::operator*(double lhs, const rl::Quaternion& rhs)
{
	Quaternion q(0, 0, 0, 1);
	q.m_data = rhs.m_data * lhs;

	return q;
}

rl::Quaternion rl::operator/(const rl::Quaternion& lhs, const rl::Quaternion& rhs)
{
	return lhs * rhs.cconjugate();
}

float rl::operator&(const rl::Quaternion &lhs, const rl::Quaternion &rhs)
{
	return lhs.magnitude() * rhs.magnitude();
}

rl::Quaternion& rl::Quaternion::operator+=(const Quaternion &rhs)
{
	m_data += rhs.m_data;
	return *this;
}

rl::Quaternion& rl::Quaternion::operator-=(const Quaternion &rhs)
{
	m_data -= rhs.m_data;
	return *this;
}

rl::Quaternion& rl::Quaternion::operator*=(const Quaternion &rhs)
{
	*this = *this * rhs;
	return *this;
}

rl::Quaternion& rl::Quaternion::operator*=(float rhs)
{
	m_data *= rhs;
	return *this;
}

rl::Quaternion& rl::Quaternion::operator/=(const Quaternion &rhs)
{
	*this = *this / rhs;
	return *this;
}
