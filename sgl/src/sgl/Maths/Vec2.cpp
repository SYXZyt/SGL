#include "Vec2.h"
#include <sgl/Maths/Maths.h>

namespace sgl
{
	const Vec2& Vec2::Zero = { 0, 0 };
	const Vec2& Vec2::One = { 1, 1 };
	const Vec2& Vec2::Left = { -1, 0 };
	const Vec2& Vec2::Right = { 1, 0 };
	const Vec2& Vec2::Up = { 0, 1 };
	const Vec2& Vec2::Down = { 0, -1 };
	const Vec2& Vec2::Infinity = { Maths::INF, Maths::INF };
	const Vec2& Vec2::NegativeInfinity = { Maths::NEG_INF, Maths::NEG_INF };

	Vec2 Vec2::operator+(const Vec2& other) const {
		return { x + other.x, y + other.y };
	}

	Vec2 Vec2::operator-(const Vec2& other) const {
		return { x - other.x, y - other.y };
	}

	Vec2 Vec2::operator*(const Vec2& other) const {
		return { x * other.x, y * other.y };
	}

	Vec2 Vec2::operator/(const Vec2& other) const {
		return { x / other.x, y / other.y };
	}

	Vec2 Vec2::operator*(float scalar) const {
		return { x * scalar, y * scalar };
	}

	Vec2 Vec2::operator/(float scalar) const {
		return { x / scalar, y / scalar };
	}

	Vec2 Vec2::operator-() const {
		return { -x, -y };
	}

	Vec2& Vec2::operator+=(const Vec2& other)
	{
		x += other.x;
		y += other.y;

		return *this;
	}

	Vec2& Vec2::operator-=(const Vec2& other)
	{
		x -= other.x;
		y -= other.y;

		return *this;
	}

	Vec2& Vec2::operator*=(const Vec2& other)
	{
		x *= other.x;
		y *= other.y;

		return *this;
	}

	Vec2& Vec2::operator/=(const Vec2& other)
	{
		x /= other.x;
		y /= other.y;

		return *this;
	}

	Vec2& Vec2::operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;

		return *this;
	}

	Vec2& Vec2::operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;

		return *this;
	}

	bool Vec2::operator==(const Vec2& other) const {
		return x == other.x && y == other.y;
	}

	bool Vec2::operator!=(const Vec2& other) const {
		return !(*this == other);
	}

	Vec2& Vec2::operator=(const Vec2& other)
	{
		x = other.x;
		y = other.y;

		return *this;
	}

	Vec2::Vec2() : Vec2(0) {}
	Vec2::Vec2(float scalar) : Vec2(scalar, scalar) {}
	Vec2::Vec2(float x, float y) : x(x), y(y) {}

	Vec2::Vec2(int8  x, int8  y) : Vec2((float)x, (float)y) {}
	Vec2::Vec2(int16 x, int16 y) : Vec2((float)x, (float)y) {}
	Vec2::Vec2(int32 x, int32 y) : Vec2((float)x, (float)y) {}
	Vec2::Vec2(int64 x, int64 y) : Vec2((float)x, (float)y) {}

	Vec2::Vec2(uint8  x, uint8  y) : Vec2((float)x, (float)y) {}
	Vec2::Vec2(uint16 x, uint16 y) : Vec2((float)x, (float)y) {}
	Vec2::Vec2(uint32 x, uint32 y) : Vec2((float)x, (float)y) {}
	Vec2::Vec2(uint64 x, uint64 y) : Vec2((float)x, (float)y) {}

	Vec2::Vec2(const Vec2& other) : Vec2(other.x, other.y) {}
}