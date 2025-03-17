#include "quaternion.h"

#include <algorithm>
#include <raylib.h>

rl::Quaternion::Quaternion(float x, float y, float z, float w)
	: m_data{ x, y, z, w }
{
}

rl::Quaternion rl::Quaternion::fromEuler(float x, float y, float z)
{
	float c1 = cos(x / 2);
	float c2 = cos(y / 2);
	float c3 = cos(z / 2);
	float s1 = sin(x / 2);
	float s2 = sin(y / 2);
	float s3 = sin(z / 2);

	return rl::Quaternion(
		s1 * c2 * c3 + c1 * s2 * s3,
		c1 * s2 * c3 - s1 * c2 * s3,
		c1 * c2 * s3 + s1 * s2 * c3,
		c1 * c2 * c3 - s1 * s2 * s3
	);
}

std::array<float, 3> rl::Quaternion::toEuler() const
{
	std::array<float, 3> euler;
	euler[0] = atan2(2 * (m_data[0] * m_data[1] + m_data[2] * m_data[3]), 1 - 2 * (m_data[1] * m_data[1] + m_data[2] * m_data[2]));
	euler[1] = asin(2 * (m_data[0] * m_data[2] - m_data[3] * m_data[1]));
	euler[2] = atan2(2 * (m_data[0] * m_data[3] + m_data[1] * m_data[2]), 1 - 2 * (m_data[2] * m_data[2] + m_data[3] * m_data[3]));
	return euler;
}

rl::Quaternion rl::Quaternion::fromRotMatrix(::Matrix matrix)
{
	float trace = matrix.m0 + matrix.m5 + matrix.m10;
	if (trace > 0) {
		float s = 0.5 / sqrt(trace + 1.0);
		return rl::Quaternion(
			(matrix.m6 - matrix.m9) * s,
			(matrix.m8 - matrix.m2) * s,
			(matrix.m1 - matrix.m4) * s,
			0.25 / s
		);
	}
	else if (matrix.m0 > matrix.m5 && matrix.m0 > matrix.m10) {
		float s = 2.0 * sqrt(1.0 + matrix.m0 - matrix.m5 - matrix.m10);
		return rl::Quaternion(
			0.25 * s,
			(matrix.m4 + matrix.m1) / s,
			(matrix.m8 + matrix.m2) / s,
			(matrix.m6 - matrix.m9) / s
		);
	}
	else if (matrix.m5 > matrix.m10) {
		float s = 2.0 * sqrt(1.0 + matrix.m5 - matrix.m0 - matrix.m10);
		return rl::Quaternion(
			(matrix.m4 + matrix.m1) / s,
			0.25 * s,
			(matrix.m9 + matrix.m6) / s,
			(matrix.m8 - matrix.m2) / s
		);
	}
	else {
		float s = 2.0 * sqrt(1.0 + matrix.m10 - matrix.m0 - matrix.m5);
		return rl::Quaternion(
			(matrix.m8 + matrix.m2) / s,
			(matrix.m9 + matrix.m6) / s,
			0.25 * s,
			(matrix.m1 - matrix.m4) / s
		);
	}
}

::Matrix rl::Quaternion::toRotMatrix() const
{
	::Matrix matrix = { 0 };
	matrix.m0 = 1 - 2 * m_data[1] * m_data[1] - 2 * m_data[2] * m_data[2];
	matrix.m1 = 2 * m_data[0] * m_data[1] + 2 * m_data[2] * m_data[3];
	matrix.m2 = 2 * m_data[0] * m_data[2] - 2 * m_data[1] * m_data[3];
	matrix.m4 = 2 * m_data[0] * m_data[1] - 2 * m_data[2] * m_data[3];
	matrix.m5 = 1 - 2 * m_data[0] * m_data[0] - 2 * m_data[2] * m_data[2];
	matrix.m6 = 2 * m_data[1] * m_data[2] + 2 * m_data[0] * m_data[3];
	matrix.m8 = 2 * m_data[0] * m_data[2] + 2 * m_data[1] * m_data[3];
	matrix.m9 = 2 * m_data[1] * m_data[2] - 2 * m_data[0] * m_data[3];
	matrix.m10 = 1 - 2 * m_data[0] * m_data[0] - 2 * m_data[1] * m_data[1];
	return matrix;
}

std::array<float, 4> rl::Quaternion::data() const
{
	return m_data;
}

double rl::Quaternion::magnitude() const
{
	return sqrt(m_data[0] * m_data[0] + m_data[1] * m_data[1] + m_data[2] * m_data[2] + m_data[3] * m_data[3]);
}

rl::Quaternion rl::Quaternion::inverse() const
{
	double gain = 1.0 / (m_data[0] * m_data[0] + m_data[1] * m_data[1] + m_data[2] * m_data[2] + m_data[3] * m_data[3]);
	return  gain * this->conjugate();
}

rl::Quaternion rl::Quaternion::conjugate() const
{
	return Quaternion(-m_data[0], -m_data[1], -m_data[2], m_data[3]);
}

rl::Quaternion rl::Quaternion::normalize() const
{
	double gain = 1.0 / magnitude();
	return gain * *this;
}

rl::Quaternion rl::Quaternion::rotate(const Quaternion& q) const
{
	return *this * q * this->conjugate();
}

rl::Quaternion &rl::Quaternion::inverse()
{
	double gain = 1.0 / (m_data[0] * m_data[0] + m_data[1] * m_data[1] + m_data[2] * m_data[2] + m_data[3] * m_data[3]);
	*this = gain * this->conjugate();
	return *this;
}

rl::Quaternion &rl::Quaternion::conjugate()
{
	m_data = { -m_data[0], -m_data[1], -m_data[2], m_data[3] };
	return *this;
}

rl::Quaternion &rl::Quaternion::normalize()
{
	double gain = 1.0 / magnitude();
	*this = gain * *this;
	return *this;
}

rl::Quaternion &rl::Quaternion::rotate(const Quaternion& q)
{
	*this *= q * this->conjugate();
	return *this;
}

rl::Quaternion rl::operator+(const rl::Quaternion& lhs, const rl::Quaternion& rhs)
{
	return Quaternion(lhs.m_data[0] + rhs.m_data[0], lhs.m_data[1] + rhs.m_data[1], lhs.m_data[2] + rhs.m_data[2], lhs.m_data[3] + rhs.m_data[3]);
}

rl::Quaternion rl::operator-(const rl::Quaternion& lhs, const rl::Quaternion& rhs)
{
	return Quaternion(lhs.m_data[0] - rhs.m_data[0], lhs.m_data[1] - rhs.m_data[1], lhs.m_data[2] - rhs.m_data[2], lhs.m_data[3] - rhs.m_data[3]);
}

rl::Quaternion rl::operator*(const rl::Quaternion& lhs, const rl::Quaternion& rhs)
{
	return rl::Quaternion(
		lhs.m_data[3] * rhs.m_data[0] + lhs.m_data[0] * rhs.m_data[3] + lhs.m_data[1] * rhs.m_data[2] - lhs.m_data[2] * rhs.m_data[1],
		lhs.m_data[3] * rhs.m_data[1] - lhs.m_data[0] * rhs.m_data[2] + lhs.m_data[1] * rhs.m_data[3] + lhs.m_data[2] * rhs.m_data[0],
		lhs.m_data[3] * rhs.m_data[2] + lhs.m_data[0] * rhs.m_data[1] - lhs.m_data[1] * rhs.m_data[0] + lhs.m_data[2] * rhs.m_data[3],
		lhs.m_data[3] * rhs.m_data[3] - lhs.m_data[0] * rhs.m_data[0] - lhs.m_data[1] * rhs.m_data[1] - lhs.m_data[2] * rhs.m_data[2]
	);
}

rl::Quaternion rl::operator*(const rl::Quaternion& lhs, double rhs)
{
	return Quaternion(lhs.m_data[0] * rhs, lhs.m_data[1] * rhs, lhs.m_data[2] * rhs, lhs.m_data[3] * rhs);
}

rl::Quaternion rl::operator*(double lhs, const rl::Quaternion& rhs)
{
	return Quaternion(rhs.m_data[0] * lhs, rhs.m_data[1] * lhs, rhs.m_data[2] * lhs, rhs.m_data[3] * lhs);
}

rl::Quaternion rl::operator/(const rl::Quaternion& lhs, const rl::Quaternion& rhs)
{
	return lhs * rhs.inverse();
}

rl::Quaternion& rl::Quaternion::operator+=(const Quaternion &rhs)
{
	for (int i = 0; i < m_data.size(); i++)
		m_data[i] += rhs.m_data[i];

	return *this;
}

rl::Quaternion& rl::Quaternion::operator-=(const Quaternion &rhs)
{
	for (int i = 0; i < m_data.size(); i++)
		m_data[i] += rhs.m_data[i];

	return *this;
}

rl::Quaternion& rl::Quaternion::operator*=(const Quaternion &rhs)
{
	*this = *this * rhs;
	return *this;
}

rl::Quaternion& rl::Quaternion::operator*=(double rhs)
{
	for (int i = 0; i < m_data.size(); i++)
		m_data[i] *= rhs;

	return *this;
}

rl::Quaternion& rl::Quaternion::operator/=(const Quaternion &rhs)
{
	*this = *this / rhs;
	return *this;
}
