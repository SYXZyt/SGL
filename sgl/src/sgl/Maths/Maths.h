#pragma once
#include <sgl/sgl.h>
#include <cmath>
#include <limits>
#include <numbers>
#include <sgl/Maths/Vec2.h>

constexpr float sqrtNewtonRaphson(float x, float curr, float prev) {
    return curr == prev ? curr : sqrtNewtonRaphson(x, 0.5f * (curr + x / curr), curr);
}

namespace sgl::Maths
{
    inline constexpr float PI = std::numbers::pi_v<float>;
    inline constexpr float HALF_PI = PI * 0.5f;
    inline constexpr float TWO_PI = PI * 2.f;
    inline constexpr float TAU = TWO_PI;

    inline constexpr float DEG2RAD = PI / 180.f;
    inline constexpr float RAD2DEG = 180.f / PI;

    inline constexpr float EPS = std::numeric_limits<float>::epsilon();
    inline constexpr float INF = std::numeric_limits<float>::infinity();
    inline constexpr float NEG_INF = -INF;
    inline constexpr float NaN = std::numeric_limits<float>::quiet_NaN();

    constexpr bool IsPowerOfTwo(const int x) {
        return (x > 0) && ((x & (x - 1)) == 0);
    }

    //https://stackoverflow.com/questions/8622256/in-c11-is-sqrt-defined-as-constexpr
    constexpr float Sqrt(float x) {
        return x >= 0 && x < INF ? sqrtNewtonRaphson(x, x, 0) : NaN;
    }

    constexpr float Sq(float x) {
        return x * x;
    }

	constexpr int Sq(int x) {
		return x * x;
	}

    constexpr bool IsMultiple(float value, float multiple) {
		return (int)value % (int)multiple == 0;
	}

	constexpr float Lerp(float a, float b, float t) {
		return a * (1.f - t) + b * t;
	}

	constexpr float Min(float a, float b) {
		return a < b ? a : b;
	}

	constexpr float Max(float a, float b) {
		return a > b ? a : b;
	}

	constexpr int Min(int a, int b) {
		return a < b ? a : b;
	}

	constexpr int Max(int a, int b) {
		return a > b ? a : b;
	}

	constexpr float Clamp(float value, float min, float max) {
		return Max(min, Min(max, value));
	}

	constexpr int Clamp(int value, int min, int max) {
		return Max(min, Min(max, value));
	}

	constexpr float Abs(float f) {
		return f < 0 ? -f : f;
	}

	constexpr float Copysign(float f, float sign) {
		return (sign < 0) ? -Abs(f) : Abs(f);
	}

	constexpr float Trunc(float f) {
		return f < 0 ? (float)(int)(f + 1.f) : (float)(int)(f);
	}

	constexpr float Rad(float deg) {
		return deg * DEG2RAD;
	}

	constexpr float Deg(float rad) {
		return rad * RAD2DEG;
	}

	constexpr float Normalise(float f, float min, float max) {
		return (f - min) / (max - min);
	}

	constexpr bool FloatEquals(float a, float b, float epsilon = EPS) {
		return Abs(a - b) <= epsilon;
	}

	constexpr bool IsNan(float f) {
		return f != f;
	}

	SGL_API extern float Sin(float f);
	SGL_API extern float Sin(float f, float min, float max);

	SGL_API extern float Cos(float f);
	SGL_API extern float Cos(float f, float min, float max);

	SGL_API extern float Tan(float f);

	SGL_API extern float ASin(float f);
	SGL_API extern float ACos(float f);
	SGL_API extern float ATan(float f);
	SGL_API extern float ATan2(float y, float x);

	SGL_API extern float Log(float f);

	SGL_API extern float Floor(float f);
	SGL_API extern float Ceil(float f);
	SGL_API extern float Round(float f);

	SGL_API extern float Pow(float base, float exponent);

	SGL_API extern float LerpDT(float a, float b, float t, float dt);


	SGL_API extern float Dist(const Vec2& a, const Vec2& b);
	SGL_API extern float Dist2(const Vec2& a, const Vec2& b);

	SGL_API extern float Angle(const Vec2& a, const Vec2& b);

	SGL_API extern float Direction(const Vec2& v);

	SGL_API extern float Length(const Vec2& v);
	SGL_API extern float Length2(const Vec2& v);

	SGL_API extern float Dot(const Vec2& a, const Vec2& b);

	SGL_API extern Vec2 Normalise(const Vec2& v);

	SGL_API extern float AspectRatio(const Vec2& v);

	SGL_API extern Vec2 Clamp(const Vec2& v, const Vec2& min, const Vec2& max);

	SGL_API extern Vec2 Floor(const Vec2& v);

	SGL_API extern bool IsNan(const Vec2& v);


	SGL_API extern float Dist(const Vec2i& a, const Vec2i& b);
	SGL_API extern float Dist2(const Vec2i& a, const Vec2i& b);

	SGL_API extern float Angle(const Vec2i& a, const Vec2i& b);

	SGL_API extern float Direction(const Vec2i& v);

	SGL_API extern float Length(const Vec2i& v);
	SGL_API extern float Length2(const Vec2i& v);

	SGL_API extern float Dot(const Vec2i& a, const Vec2i& b);

	SGL_API extern float AspectRatio(const Vec2i& v);

	SGL_API extern Vec2i Clamp(const Vec2i& v, const Vec2i& min, const Vec2i& max);
}