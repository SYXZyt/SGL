#include "Vec2.h"
#include <sgl/Maths/Maths.h>

namespace sgl
{
#pragma region Float
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
	Vec2::Vec2(double x, double y) : x((float)x), y((float)y) {}

	Vec2::Vec2(int8  x, int8  y) : Vec2((float)x, (float)y) {}
	Vec2::Vec2(int16 x, int16 y) : Vec2((float)x, (float)y) {}
	Vec2::Vec2(int32 x, int32 y) : Vec2((float)x, (float)y) {}
	Vec2::Vec2(int64 x, int64 y) : Vec2((float)x, (float)y) {}

	Vec2::Vec2(uint8  x, uint8  y) : Vec2((float)x, (float)y) {}
	Vec2::Vec2(uint16 x, uint16 y) : Vec2((float)x, (float)y) {}
	Vec2::Vec2(uint32 x, uint32 y) : Vec2((float)x, (float)y) {}
	Vec2::Vec2(uint64 x, uint64 y) : Vec2((float)x, (float)y) {}

	Vec2::Vec2(const Vec2& other) : Vec2(other.x, other.y) {}
#pragma endregion

#pragma region Int

	const Vec2i& Vec2i::Zero = { 0, 0 };
	const Vec2i& Vec2i::One = { 1, 1 };
	const Vec2i& Vec2i::Left = { -1, 0 };
	const Vec2i& Vec2i::Right = { 1, 0 };
	const Vec2i& Vec2i::Up = { 0, 1 };
	const Vec2i& Vec2i::Down = { 0, -1 };
	const Vec2i& Vec2i::Infinity = { Maths::INF, Maths::INF };
	const Vec2i& Vec2i::NegativeInfinity = { Maths::NEG_INF, Maths::NEG_INF };

	Vec2i Vec2i::operator+(const Vec2i& other) const {
		return { x + other.x, y + other.y };
	}

	Vec2i Vec2i::operator-(const Vec2i& other) const {
		return { x - other.x, y - other.y };
	}

	Vec2i Vec2i::operator*(const Vec2i& other) const {
		return { x * other.x, y * other.y };
	}

	Vec2i Vec2i::operator/(const Vec2i& other) const {
		return { x / other.x, y / other.y };
	}

	Vec2i Vec2i::operator*(int scalar) const {
		return { x * scalar, y * scalar };
	}

	Vec2i Vec2i::operator/(int scalar) const {
		return { x / scalar, y / scalar };
	}

	Vec2i Vec2i::operator-() const {
		return { -x, -y };
	}

	Vec2i& Vec2i::operator+=(const Vec2i& other)
	{
		x += other.x;
		y += other.y;

		return *this;
	}

	Vec2i& Vec2i::operator-=(const Vec2i& other)
	{
		x -= other.x;
		y -= other.y;

		return *this;
	}

	Vec2i& Vec2i::operator*=(const Vec2i& other)
	{
		x *= other.x;
		y *= other.y;

		return *this;
	}

	Vec2i& Vec2i::operator/=(const Vec2i& other)
	{
		x /= other.x;
		y /= other.y;

		return *this;
	}

	Vec2i& Vec2i::operator*=(int scalar)
	{
		x *= scalar;
		y *= scalar;

		return *this;
	}

	Vec2i& Vec2i::operator/=(int scalar)
	{
		x /= scalar;
		y /= scalar;

		return *this;
	}

	bool Vec2i::operator==(const Vec2i& other) const {
		return x == other.x && y == other.y;
	}

	bool Vec2i::operator!=(const Vec2i& other) const {
		return !(*this == other);
	}

	Vec2i& Vec2i::operator=(const Vec2i& other)
	{
		x = other.x;
		y = other.y;

		return *this;
	}

	Vec2i::Vec2i() : Vec2i(0) {}
	Vec2i::Vec2i(int scalar) : Vec2i(scalar, scalar) {}
	Vec2i::Vec2i(float x, float y) : x((int)x), y((int)y) {}
	Vec2i::Vec2i(double x, double y) : x((int)x), y((int)y) {}

	Vec2i::Vec2i(int8  x, int8  y) : Vec2i((int)x, (int)y) {}
	Vec2i::Vec2i(int16 x, int16 y) : Vec2i((int)x, (int)y) {}
	Vec2i::Vec2i(int32 x, int32 y) : x(x), y(y) {}
	Vec2i::Vec2i(int64 x, int64 y) : Vec2i((int)x, (int)y) {}

	Vec2i::Vec2i(uint8  x, uint8  y) : Vec2i((int)x, (int)y) {}
	Vec2i::Vec2i(uint16 x, uint16 y) : Vec2i((int)x, (int)y) {}
	Vec2i::Vec2i(uint32 x, uint32 y) : Vec2i((int)x, (int)y) {}
	Vec2i::Vec2i(uint64 x, uint64 y) : Vec2i((int)x, (int)y) {}

	Vec2i::Vec2i(const Vec2i& other) : Vec2i(other.x, other.y) {}

#pragma endregion
}