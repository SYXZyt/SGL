#include "Maths.h"

namespace sgl::Maths
{
    float Sin(float f) {
		return std::sinf(f);
	}

	float Sin(float f, float min, float max)
	{
		float amplitude = (max - min) * 0.5f;
		float mid = (max + min) * 0.5f;
		return Sin(f) * amplitude + mid;
	}

	float Cos(float f) {
		return std::cosf(f);
	}

	float Cos(float f, float min, float max)
	{
		float amplitude = (max - min) * 0.5f;
		float mid = (max + min) * 0.5f;
		return Cos(f) * amplitude + mid;
	}

	float Tan(float f) {
		return std::tanf(f);
	}

	float ASin(float f) {
		return std::asinf(f);
	}

	float ACos(float f) {
		return std::acosf(f);
	}

	float ATan(float f) {
		return std::atanf(f);
	}

	float ATan2(float y, float x) {
		return std::atan2f(y, x);
	}

	float Log(float f) {
		return std::logf(f);
	}

	float Floor(float f) {
		return std::floorf(f);
	}

	float Ceil(float f) {
		return std::ceilf(f);
	}

	float Round(float f) {
		return std::roundf(f);
	}

	float Pow(float base, float exponent) {
		return std::powf(base, exponent);
	}

	float LerpDT(float a, float b, float t, float dt) {
		return Lerp(a, b, 1 - Pow(t, dt));
	}

	float Dist(const Vec2& a, const Vec2& b) {
		return Sqrt(Dist2(a, b));
	}

	float Dist2(const Vec2& a, const Vec2& b)
	{
		const float dx = b.x - a.x;
		const float dy = b.y - a.y;

		return dx * dx + dy * dy;
	}

	float Angle(const Vec2& a, const Vec2& b)
	{
		const float dot = Dot(a, b);
		const float lengths = Length(a) * Length(b);

		return ACos(dot / lengths);
	}

	float Direction(const Vec2& v) {
		return ATan2(v.y, v.x);
	}

	float Length(const Vec2& v) {
		return Sqrt(Length2(v));
	}

	float Length2(const Vec2& v) {
		return Sq(v.x) + Sq(v.y);
	}

	float Dot(const Vec2& a, const Vec2& b) {
		return a.x * b.x + a.y * b.y;
	}

	Vec2 Normalise(const Vec2& v)
	{
		const float len = Length(v);
		if (len == 0.f)
			return Vec2::Zero;

		return v / len;
	}

	float AspectRatio(const Vec2& v)
	{
		if (v.y == 0.f)
			return INF;

		return v.width / v.height;
	}

	Vec2 Clamp(const Vec2& v, const Vec2& min, const Vec2& max)
	{
		return Vec2(
			Clamp(v.x, min.x, max.x),
			Clamp(v.y, min.y, max.y)
		);
	}

	Vec2 Floor(const Vec2& v)
	{
		return Vec2(
			Floor(v.x),
			Floor(v.y)
		);
	}

	bool IsNan(const Vec2& v) {
		return std::isnan(v.x) || std::isnan(v.y);
	}

	float Dist(const Vec2i& a, const Vec2i& b) {
		return Sqrt(Dist2(a, b));
	}

	float Dist2(const Vec2i& a, const Vec2i& b)
	{
		const float dx = (float)(b.x - a.x);
		const float dy = (float)(b.y - a.y);

		return dx * dx + dy * dy;
	}

	float Angle(const Vec2i& a, const Vec2i& b)
	{
		const float dot = Dot(a, b);
		const float lengths = Length(a) * Length(b);

		return ACos(dot / lengths);
	}

	float Direction(const Vec2i& v) {
		return ATan2((float)v.y, (float)v.x);
	}

	float Length(const Vec2i& v) {
		return Sqrt(Length2(v));
	}

	float Length2(const Vec2i& v) {
		return (float)(Sq(v.x) + Sq(v.y));
	}

	float Dot(const Vec2i& a, const Vec2i& b) {
		return (float)(a.x * b.x + a.y * b.y);
	}

	float AspectRatio(const Vec2i& v)
	{
		if (v.y == 0.f)
			return INF;

		return (float)v.width / (float)v.height;
	}

	Vec2i Clamp(const Vec2i& v, const Vec2i& min, const Vec2i& max)
	{
		return Vec2i(
			Clamp(v.x, min.x, max.x),
			Clamp(v.y, min.y, max.y)
		);
	}
}