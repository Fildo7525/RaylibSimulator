#include "quaternion.h"
#include <raymath.h>

Matrix3f rl::skewMarix(const Vector3f &v)
{
	Matrix3f skew;
	skew << 0, -v.z(), v.y(),
			v.z(), 0, -v.x(),
			-v.y(), v.x(), 0;
	return skew;
}

Vector3 rl::Quaternion::toRlVector3() const
{
	return Vector3{ m_data.x(), m_data.y(), m_data.z() };
}

rl::Quaternion::Quaternion(float x, float y, float z, float w)
	: m_data{ x, y, z, w }
{
}

rl::Quaternion rl::Quaternion::fromEuler(float x, float y, float z)
{
	return Quaternion::fromEuler(Vector3{x, y, z});
}

rl::Quaternion rl::Quaternion::fromEuler(const Vector3f &euler)
{
	return Quaternion(QuaternionFromEuler(euler.x(), euler.y(), euler.z()));
}

rl::Quaternion rl::Quaternion::fromEuler(const Vector3 &euler)
{
	return Quaternion(QuaternionFromEuler(euler.x, euler.y, euler.z));
}

Vector3f rl::Quaternion::toEuler(bool degrees) const
{
	::Quaternion quat{m_data.x(), m_data.y(), m_data.z(), m_data.w()};
	Vector3 angles = QuaternionToEuler(quat);
	Vector3f euler = { angles.y, angles.z, angles.x };
	euler *= (degrees ? 180.0f / PI : 1.0f);
	return euler;
}

rl::Quaternion rl::Quaternion::fromEigRotMatrix(const Matrix4f &matrix)
{
	::Matrix mat = { matrix(0, 0), matrix(0, 1), matrix(0, 2), matrix(0, 3),
					 matrix(1, 0), matrix(1, 1), matrix(1, 2), matrix(1, 3),
					 matrix(2, 0), matrix(2, 1), matrix(2, 2), matrix(2, 3),
					 matrix(3, 0), matrix(3, 1), matrix(3, 2), matrix(3, 3) };

	return Quaternion(QuaternionFromMatrix(mat));
}

rl::Quaternion rl::Quaternion::fromRlRotMatrix(const ::Matrix &matrix)
{
	return Quaternion(QuaternionFromMatrix(matrix));
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

Matrix3f rl::Quaternion::toRotationMatrix() const
{
	auto nu = m_data.w();
	auto eta = m_data(Eigen::seq(0, 2));

	Matrix3f etaSkew = skewMarix(eta);
	auto R = Matrix3f::Identity() + 2 * nu * etaSkew + 2 * etaSkew * etaSkew;
	return R;
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

rl::Quaternion rl::Quaternion::rotate(const Quaternion& q) const
{
	return *this * (q * cconjugate());
}

rl::Quaternion rl::Quaternion::rotate(const Vector3f &v) const
{
	return rotate(Quaternion(v(0), v(1), v(2), 0));
}

rl::Quaternion rl::Quaternion::rotate(const Vector3 &v) const
{
	return rotate(Quaternion(v.x, v.y, v.z, 0));
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
