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

	struct SGL_API Vec2i final
	{
		static const Vec2i& Zero;
		static const Vec2i& One;
		static const Vec2i& Left;
		static const Vec2i& Right;
		static const Vec2i& Up;
		static const Vec2i& Down;
		static const Vec2i& Infinity;
		static const Vec2i& NegativeInfinity;

		union
		{
			struct { int x, y; };
			struct { int u, v; };
			struct { int width, height; };
			int values[2];
		};

		Vec2i operator+(const Vec2i& other) const;
		Vec2i operator-(const Vec2i& other) const;
		Vec2i operator*(const Vec2i& other) const;
		Vec2i operator/(const Vec2i& other) const;

		Vec2i operator*(int scalar) const;
		Vec2i operator/(int scalar) const;

		Vec2i operator-() const;

		Vec2i& operator+=(const Vec2i& other);
		Vec2i& operator-=(const Vec2i& other);
		Vec2i& operator*=(const Vec2i& other);
		Vec2i& operator/=(const Vec2i& other);

		Vec2i& operator*=(int scalar);
		Vec2i& operator/=(int scalar);

		bool operator==(const Vec2i& other) const;
		bool operator!=(const Vec2i& other) const;

		Vec2i& operator=(const Vec2i& other);

		Vec2i();
		Vec2i(int scalar);
		Vec2i(float x, float y);
		Vec2i(double x, double y);

		Vec2i(int8 x, int8 y);
		Vec2i(int16 x, int16 y);
		Vec2i(int32 x, int32 y);
		Vec2i(int64 x, int64 y);

		Vec2i(uint8 x, uint8 y);
		Vec2i(uint16 x, uint16 y);
		Vec2i(uint32 x, uint32 y);
		Vec2i(uint64 x, uint64 y);

		Vec2i(const Vec2i& other);
	};
}