#pragma once
#include <sgl/sgl.h>

namespace sgl
{
    struct SGL_API Vec2 final
    {
        static const Vec2& Zero;
        static const Vec2& One;
        static const Vec2& Left;
        static const Vec2& Right;
        static const Vec2& Up;
        static const Vec2& Down;
        static const Vec2& Infinity;
        static const Vec2& NegativeInfinity;

        union
        {
            struct { float x, y; };
            struct { float u, v; };
            struct { float width, height; };
            float values[2];
        };

        Vec2 operator+(const Vec2& other) const;
		Vec2 operator-(const Vec2& other) const;
		Vec2 operator*(const Vec2& other) const;
		Vec2 operator/(const Vec2& other) const;

		Vec2 operator*(float scalar) const;
		Vec2 operator/(float scalar) const;

		Vec2 operator-() const;

		Vec2& operator+=(const Vec2& other);
		Vec2& operator-=(const Vec2& other);
		Vec2& operator*=(const Vec2& other);
		Vec2& operator/=(const Vec2& other);

		Vec2& operator*=(float scalar);
		Vec2& operator/=(float scalar);

		bool operator==(const Vec2& other) const;
		bool operator!=(const Vec2& other) const;

		Vec2& operator=(const Vec2& other);

		Vec2();
		Vec2(float scalar);
		Vec2(float x, float y);
        Vec2(double x, double y);

		Vec2(int8 x, int8 y);
		Vec2(int16 x, int16 y);
		Vec2(int32 x, int32 y);
		Vec2(int64 x, int64 y);

		Vec2(uint8 x, uint8 y);
		Vec2(uint16 x, uint16 y);
		Vec2(uint32 x, uint32 y);
		Vec2(uint64 x, uint64 y);

		Vec2(const Vec2& other);
    };
}