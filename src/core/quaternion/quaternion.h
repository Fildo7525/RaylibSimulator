#pragma once

#include <print>

#include <raylib.h>
#include <raymath.h>
#include <Eigen/Dense>

using Vector6f = Eigen::Matrix<float, 6, 1>;
using Eigen::Vector3f;
using Eigen::Matrix4f;
using Eigen::Vector4f;

namespace rl
{

class Quaternion
{
	friend Quaternion operator+(const Quaternion &lhs, const Quaternion &rhs);
	friend Quaternion operator-(const Quaternion &lhs, const Quaternion &rhs);
	friend Quaternion operator*(const Quaternion &lhs, const Quaternion &rhs);
	friend Quaternion operator*(const Quaternion &lhs, double rhs);
	friend Quaternion operator*(double lhs, const Quaternion &rhs);
	friend Quaternion operator/(const Quaternion &lhs, const Quaternion &rhs);
	friend float operator&(const Quaternion &lhs, const Quaternion &rhs);

private:
	Eigen::Vector4f toEigVector() const;
	Eigen::Vector4f toEigVector(const ::Quaternion &quat) const;

public:
	Quaternion(float x, float y, float z, float w);

	float x() const { return m_data.x(); }
	float y() const { return m_data.y(); }
	float z() const { return m_data.z(); }
	float w() const { return m_data.w(); }

	static rl::Quaternion fromEuler(float x, float y, float z);

	Vector3f toEuler() const;

	static rl::Quaternion fromEigRotMatrix(Matrix4f matrix);
	static rl::Quaternion fromRlRotMatrix(::Matrix matrix);

	::Quaternion toRlQuaternion() const;
	::Matrix toRlRotMatrix() const;
	Matrix4f toEigRotMatrix() const;

	Vector4f data() const;

	float magnitude() const;

	Quaternion cconjugate() const;
	Quaternion cnormalize() const;
	Quaternion crotate(const Quaternion &q) const;
	Quaternion ctranspose() const;
	float dot(const Quaternion &q) const;

	Quaternion &conjugate();
	Quaternion &normalize();
	Quaternion &rotate(const Quaternion &q);

	Quaternion &operator+=(const Quaternion &rhs);
	Quaternion &operator-=(const Quaternion &rhs);
	Quaternion &operator*=(const Quaternion &rhs);
	Quaternion &operator*=(float rhs);
	Quaternion &operator/=(const Quaternion &rhs);

private:

	Vector4f m_data;
};

Quaternion operator+(const Quaternion &lhs, const Quaternion &rhs);
Quaternion operator-(const Quaternion &lhs, const Quaternion &rhs);
Quaternion operator*(const Quaternion &lhs, const Quaternion &rhs);
Quaternion operator*(const Quaternion &lhs, double rhs);
Quaternion operator*(double lhs, const Quaternion& rhs);
Quaternion operator/(const Quaternion &lhs, const Quaternion &rhs);
float operator&(const Quaternion &lhs, const Quaternion &rhs);
} // namespace RLC

template <>
struct std::formatter<rl::Quaternion> {
	constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const rl::Quaternion& p, FormatContext& ctx) const {
		auto data = p.data();
		return std::format_to(ctx.out(), "Quaternion({}, {}, {}, {})", data[0], data[1], data[2], data[3]);
	}
};


std::ostream &operator<<(std::ostream &os, const rl::Quaternion &q);
