#pragma once

#include <print>

#include <raylib.h>
#include <raymath.h>
#include <Eigen/Dense>

using Vector6f = Eigen::Matrix<float, 6, 1>;
using Eigen::Vector3f;
using Eigen::Vector4f;
using Eigen::Matrix3f;
using Eigen::Matrix4f;

namespace rl
{

Matrix3f skewMarix(const Vector3f &v);

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
	Eigen::Vector4f toEigVector(const ::Quaternion &quat) const;

public:
	Quaternion(float x, float y, float z, float w);

	float x() const { return m_data.x(); }
	float y() const { return m_data.y(); }
	float z() const { return m_data.z(); }
	float w() const { return m_data.w(); }

	static rl::Quaternion fromEuler(float x, float y, float z);

	Vector3f toEuler(bool degrees = false) const;

	static rl::Quaternion fromEigRotMatrix(Matrix4f matrix);
	static rl::Quaternion fromRlRotMatrix(::Matrix matrix);

	::Quaternion toRlQuaternion() const;
	::Matrix toRlRotMatrix() const;
	Matrix4f toEigRotMatrix() const;
	Matrix3f toRotationMatrix() const;
	Vector4f toEigVector() const;

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
		return std::format_to(ctx.out(), "Quaternion (xyzw)  [{}, {}, {}, {}]", data.x(), data.y(), data.z(), data.w());
	}
};

template <>
struct std::formatter<Matrix3f> {
	constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const Matrix3f& p, FormatContext& ctx) const {
		return std::format_to(ctx.out(), "[ {} {} {};\n  {} {} {};\n  {} {} {} ]", p(0), p(1), p(2),
			p(3), p(4), p(5), p(6), p(7), p(8));
	}
};

std::ostream &operator<<(std::ostream &os, const rl::Quaternion &q);
