#pragma once

#include <array>
#include <cmath>
#include <format>
#include <print>

class Matrix;
namespace rlc
{

class Quaternion
{
public:
	Quaternion(float x, float y, float z, float w);

	static Quaternion fromEuler(float x, float y, float z);

	std::array<float, 3> toEuler() const;

	static Quaternion fromRotMatrix(::Matrix matrix);

	::Matrix toRotMatrix() const;

	std::array<float, 4> data() const;

	double magnitude() const;

	Quaternion inverse() const;
	Quaternion conjugate() const;
	Quaternion normalize() const;
	Quaternion rotate(const Quaternion &q) const;

	Quaternion &inverse();
	Quaternion &conjugate();
	Quaternion &normalize();
	Quaternion &rotate(const Quaternion &q);

	friend Quaternion operator+(const Quaternion &lhs, const Quaternion &rhs);
	friend Quaternion operator-(const Quaternion &lhs, const Quaternion &rhs);
	friend Quaternion operator*(const Quaternion &lhs, const Quaternion &rhs);
	friend Quaternion operator*(const Quaternion &lhs, double rhs);
	friend Quaternion operator*(double lhs, const Quaternion &rhs);
	friend Quaternion operator/(const Quaternion &lhs, const Quaternion &rhs);

	Quaternion &operator+=(const Quaternion &rhs);
	Quaternion &operator-=(const Quaternion &rhs);
	Quaternion &operator*=(const Quaternion &rhs);
	Quaternion &operator*=(double rhs);
	Quaternion &operator/=(const Quaternion &rhs);

private:
	std::array<float, 4> m_data;
};

Quaternion operator+(const Quaternion &lhs, const Quaternion &rhs);
Quaternion operator-(const Quaternion &lhs, const Quaternion &rhs);
Quaternion operator*(const Quaternion &lhs, const Quaternion &rhs);
Quaternion operator*(const Quaternion &lhs, double rhs);
Quaternion operator*(double lhs, const Quaternion& rhs);
Quaternion operator/(const Quaternion &lhs, const Quaternion &rhs);
} // namespace RLC

template <>
struct std::formatter<rlc::Quaternion> {
	constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const rlc::Quaternion& p, FormatContext& ctx) const {
		auto data = p.data();
		return std::format_to(ctx.out(), "Quaternion({}, {}, {}, {})", data[0], data[1], data[2], data[3]);
	}
};


std::ostream &operator<<(std::ostream &os, const rlc::Quaternion &q);
