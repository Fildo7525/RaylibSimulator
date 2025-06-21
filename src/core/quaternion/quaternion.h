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

/**
 * @class Quaternion
 * @brief Represents a quaternion for 3D rotations and transformations.
 *
 */
class Quaternion
{
	friend Quaternion operator+(const Quaternion &lhs, const Quaternion &rhs);
	friend Quaternion operator-(const Quaternion &lhs, const Quaternion &rhs);
	friend Quaternion operator*(const Quaternion &lhs, const Quaternion &rhs);
	friend Quaternion operator*(const Quaternion &lhs, double rhs);
	friend Quaternion operator*(double lhs, const Quaternion &rhs);
	friend Quaternion operator/(const Quaternion &lhs, const Quaternion &rhs);
	friend float operator&(const Quaternion &lhs, const Quaternion &rhs);

public:
	Quaternion(float x, float y, float z, float w);
	explicit Quaternion(::Quaternion quat)
		: m_data{ quat.x, quat.y, quat.z, quat.w }
	{
	};

	float x() const { return m_data.x(); }
	float y() const { return m_data.y(); }
	float z() const { return m_data.z(); }
	float w() const { return m_data.w(); }

	static rl::Quaternion fromEuler(float x, float y, float z);
	static rl::Quaternion fromEuler(const Vector3f &euler);
	static rl::Quaternion fromEuler(const Vector3 &euler);

	Vector3f toEuler(bool degrees = false) const;

	static rl::Quaternion fromEigRotMatrix(const Matrix4f &matrix);
	static rl::Quaternion fromRlRotMatrix(const ::Matrix &matrix);

	::Quaternion toRlQuaternion() const;
	::Matrix toRlRotMatrix() const;
	Matrix4f toEigRotMatrix() const;
	Matrix3f toRotationMatrix() const;
	Vector4f toEigVector() const;
	Vector3 toRlVector3() const;

	Vector4f data() const;

	float magnitude() const;

	Quaternion cconjugate() const;
	Quaternion cnormalize() const;
	Quaternion ctranspose() const;
	float dot(const Quaternion &q) const;

	Quaternion &conjugate();
	Quaternion &normalize();

	Quaternion rotate(const Quaternion &q) const;
	Quaternion rotate(const Vector3f &v) const;
	Quaternion rotate(const Vector3 &v) const;

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

template <>
struct std::formatter<Vector3f> {
	constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const Vector3f& p, FormatContext& ctx) const {
		return std::format_to(ctx.out(), "[ {} {} {} ]", p(1), p(2), p(2));
	}
};

template <>
struct std::formatter<Vector3> {
	constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const Vector3& p, FormatContext& ctx) const {
		return std::format_to(ctx.out(), "[ {} {} {} ]", p.x, p.y, p.z);
	}
};

std::ostream &operator<<(std::ostream &os, const rl::Quaternion &q);
