#pragma once
#include <SGL/SGL.h>
#include <SGL/Maths/Vec2.h>
#include <SGL/Maths/Vec3.h>
#include <SGL/Maths/Vec4.h>
#include <SGL/Maths/Mat4.h>

SGL_BEGIN

SGL_API extern const float sgl_Maths_PI;
SGL_API extern const float sgl_Maths_HALF_PI;
SGL_API extern const float sgl_Maths_TWO_PI;
#define sgl_Maths_TAU sgl_Maths_TWO_PI

#define sgl_Maths_DEG2RAD (sgl_Maths_PI / 180.f)
#define sgl_Maths_RAD2DEG (180.f / sgl_Maths_PI)

SGL_API extern const float sgl_Maths_EPS;
SGL_API extern const float sgl_Maths_INF;
#define sgl_Maths_NEG_INF (-sgl_Maths_INF)
SGL_API extern const float sgl_Maths_NAN;

#define sgl_Maths_IsPowerOfTwo(x) ((x) > 0) && (((x) & ((x) - 1)) == 0)

SGL_API extern float sgl_Maths_Sqrt(float x);
#define sgl_Maths_Sq(x) (x) * (x)

// Closest we got to constexpr
#define sgl_Maths_IsMultiple(value, multiple) (int)(value) % (int)(multiple) == 0;

#define sgl_Maths_Lerp(a, b, t) ((a) * (1.f - (t)) + (b) * (t))

#define sgl_Maths_Min(a, b) ((a) < (b) ? (a) : (b))
#define sgl_Maths_Max(a, b) ((a) > (b) ? (a) : (b))
#define sgl_Maths_Clamp(value, min, max) (sgl_Maths_Max((min), sgl_Maths_Min((max), (value))))

#define sgl_Maths_Abs(f) (f) < 0 ? -(f) : (f)
#define sgl_Maths_Copysign(f, sign) ((sign) < 0) ? -sgl_Maths_Abs((f)) : sgl_Maths_Abs((f))

#define sgl_Maths_Trunc(f) ((f) < 0 ? (float)(int)((f) + 1.f) : (float)(int)((f)))

#define sgl_Maths_Rad(deg) ((deg) * sgl_Maths_DEG2RAD)
#define sgl_Maths_Deg(rad) ((rad) * sgl_Maths_RAD2DEG)

#define sgl_Maths_Normalise(f, min, max) ((f) - (min)) / ((max) - (min))
#define sgl_Maths_IsNan(f) ((f) != (f))

SGL_API extern float sgl_Maths_Sin(float f);
SGL_API extern float sgl_Maths_SinRange(float f, float min, float max);

SGL_API extern float sgl_Maths_Cos(float f);
SGL_API extern float sgl_Maths_CosRange(float f, float min, float max);

SGL_API extern float sgl_Maths_Tan(float f);

SGL_API extern float sgl_Maths_ASin(float f);
SGL_API extern float sgl_Maths_ACos(float f);
SGL_API extern float sgl_Maths_ATan(float f);
SGL_API extern float sgl_Maths_ATan2(float y, float x);

SGL_API extern float sgl_Maths_Log(float f);

SGL_API extern float sgl_Maths_Floor(float f);
SGL_API extern float sgl_Maths_Ceil(float f);
SGL_API extern float sgl_Maths_Round(float f);

SGL_API extern float sgl_Maths_Pow(float base, float exponent);

SGL_API extern float sgl_Maths_LerpDT(float a, float b, float t, float dt);


#pragma region Vec2
SGL_API extern float sgl_Maths_Vec2_Dist(sgl_Vec2 a, sgl_Vec2 b);
SGL_API extern float sgl_Maths_Vec2_Dist2(sgl_Vec2 a, sgl_Vec2 b);

SGL_API extern float sgl_Maths_Vec2_Angle(sgl_Vec2 a, sgl_Vec2 b);

SGL_API extern float sgl_Maths_Vec2_Direction(sgl_Vec2 v);

SGL_API extern float sgl_Maths_Vec2_Length(sgl_Vec2 v);
SGL_API extern float sgl_Maths_Vec2_Length2(sgl_Vec2 v);

SGL_API extern float sgl_Maths_Vec2_Dot(sgl_Vec2 a, sgl_Vec2 b);

SGL_API extern float sgl_Maths_Vec2_AspectRatio(sgl_Vec2 v);

SGL_API extern sgl_Vec2 sgl_Maths_Vec2_Floor(sgl_Vec2 v);

SGL_API extern sgl_Vec2 sgl_Maths_Vec2_Normalise(sgl_Vec2 v);
#pragma endregion

#pragma region Vec2i
SGL_API extern float sgl_Maths_Vec2i_Dist(sgl_Vec2i a, sgl_Vec2i b);
SGL_API extern float sgl_Maths_Vec2i_Dist2(sgl_Vec2i a, sgl_Vec2i b);

SGL_API extern float sgl_Maths_Vec2i_Angle(sgl_Vec2i a, sgl_Vec2i b);

SGL_API extern float sgl_Maths_Vec2i_Direction(sgl_Vec2i v);

SGL_API extern float sgl_Maths_Vec2i_Length(sgl_Vec2i v);
SGL_API extern float sgl_Maths_Vec2i_Length2(sgl_Vec2i v);

SGL_API extern float sgl_Maths_Vec2i_Dot(sgl_Vec2i a, sgl_Vec2i b);

SGL_API extern float sgl_Maths_Vec2i_AspectRatio(sgl_Vec2i v);
#pragma endregion

#pragma region Vec3
SGL_API extern float sgl_Maths_Vec3_Dist(sgl_Vec3 a, sgl_Vec3 b);
SGL_API extern float sgl_Maths_Vec3_Dist2(sgl_Vec3 a, sgl_Vec3 b);

SGL_API extern float sgl_Maths_Vec3_Angle(sgl_Vec3 a, sgl_Vec3 b);

SGL_API extern float sgl_Maths_Vec3_Length(sgl_Vec3 v);
SGL_API extern float sgl_Maths_Vec3_Length2(sgl_Vec3 v);

SGL_API extern float sgl_Maths_Vec3_Dot(sgl_Vec3 a, sgl_Vec3 b);
SGL_API extern sgl_Vec3 sgl_Maths_Vec3_Cross(sgl_Vec3 a, sgl_Vec3 b);

SGL_API extern sgl_Vec3 sgl_Maths_Vec3_Normalise(sgl_Vec3 v);
#pragma endregion

#pragma region Vec3i
SGL_API extern float sgl_Maths_Vec3i_Dist(sgl_Vec3i a, sgl_Vec3i b);
SGL_API extern float sgl_Maths_Vec3i_Dist2(sgl_Vec3i a, sgl_Vec3i b);

SGL_API extern float sgl_Maths_Vec3i_Angle(sgl_Vec3i a, sgl_Vec3i b);

SGL_API extern float sgl_Maths_Vec3i_Length(sgl_Vec3i v);
SGL_API extern float sgl_Maths_Vec3i_Length2(sgl_Vec3i v);

SGL_API extern float sgl_Maths_Vec3i_Dot(sgl_Vec3i a, sgl_Vec3i b);
SGL_API extern sgl_Vec3i sgl_Maths_Vec3i_Cross(sgl_Vec3i a, sgl_Vec3i b);
#pragma endregion

#pragma region Vec4
SGL_API extern float sgl_Maths_Vec4_Dist(sgl_Vec4 a, sgl_Vec4 b);
SGL_API extern float sgl_Maths_Vec4_Dist2(sgl_Vec4 a, sgl_Vec4 b);

SGL_API extern float sgl_Maths_Vec4_Length(sgl_Vec4 v);
SGL_API extern float sgl_Maths_Vec4_Length2(sgl_Vec4 v);

SGL_API extern float sgl_Maths_Vec4_Dot(sgl_Vec4 a, sgl_Vec4 b);

SGL_API extern sgl_Vec4 sgl_Maths_Vec4_Normalise(sgl_Vec4 v);
#pragma endregion

#pragma region Mat4
SGL_API extern sgl_Mat4 sgl_Maths_Mat4_Orthographic(sgl_Vec2i screenSize, float zoom);
SGL_API extern sgl_Mat4 sgl_Maths_Mat4_OrthographicGL(sgl_Vec2i screenSize, float zoom);

SGL_API extern sgl_Mat4 sgl_Maths_Mat4_Perspective(float fovY, float aspectRatio, float nearPlane, float farPlane);

SGL_API extern sgl_Mat4 sgl_Maths_Mat4_View(sgl_Vec3 position, float angle);
SGL_API extern sgl_Mat4 sgl_Maths_Mat4_LookAt(sgl_Vec3 eye, sgl_Vec3 target, sgl_Vec3 up);

SGL_API extern sgl_Mat4 sgl_Maths_Mat4_Inverse(sgl_Mat4 mat);
SGL_API extern sgl_Mat4 sgl_Maths_Mat4_Transpose(sgl_Mat4 mat);

SGL_API extern sgl_Mat4 sgl_Maths_Mat4_Translation(sgl_Vec3 translation);
SGL_API extern sgl_Mat4 sgl_Maths_Mat4_Rotation(float angle, sgl_Vec3 axis);
SGL_API extern sgl_Mat4 sgl_Maths_Mat4_Scale(sgl_Vec3 scale);
#pragma endregion

SGL_END

// If using C++, it is nice to have a namespaced, overloaded API on top of the
// C functions above (e.g. sgl::Maths::Dist(a, b) instead of sgl_Maths_Vec3_Dist(a, b)).
#ifdef __cplusplus

namespace sgl
{
    namespace Maths
    {
        inline const float& PI = sgl_Maths_PI;
        inline const float& HALF_PI = sgl_Maths_HALF_PI;
        inline const float& TWO_PI = sgl_Maths_TWO_PI;
        inline const float& TAU = sgl_Maths_TWO_PI;

        inline const float& EPS = sgl_Maths_EPS;
        inline const float& INF = sgl_Maths_INF;
        inline const float NegInf = sgl_Maths_NEG_INF;
        // Named NaN (not NAN) since <cmath>/<math.h> may already define NAN as a macro
        inline const float& NaN = sgl_Maths_NAN;

        template<typename T>
        inline bool IsPowerOfTwo(T x) {
            return x > 0 && (x & (x - 1)) == 0;
        }

        template<typename T>
        inline T Sq(T x) {
            return x * x;
        }

        inline bool IsMultiple(int value, int multiple) {
            return value % multiple == 0;
        }

        template<typename T>
        inline T Lerp(T a, T b, float t) {
            return a * (1.f - t) + b * t;
        }

        template<typename T>
        inline T Min(T a, T b) {
            return a < b ? a : b;
        }

        template<typename T>
        inline T Max(T a, T b) {
            return a > b ? a : b;
        }

        template<typename T>
        inline T Clamp(T value, T min, T max) {
            return Max(min, Min(max, value));
        }

        template<typename T>
        inline T Abs(T f) {
            return f < 0 ? -f : f;
        }

        template<typename T>
        inline T Copysign(T f, T sign) {
            return sign < 0 ? -Abs(f) : Abs(f);
        }

        inline float Trunc(float f) {
            return sgl_Maths_Trunc(f);
        }

        inline float Rad(float deg) {
            return deg * sgl_Maths_DEG2RAD;
        }

        inline float Deg(float rad) {
            return rad * sgl_Maths_RAD2DEG;
        }

        template<typename T>
        inline T Normalise(T f, T min, T max) {
            return (f - min) / (max - min);
        }

        template<typename T>
        inline bool IsNan(T f) {
            return f != f;
        }

        inline float Sqrt(float f) { return sgl_Maths_Sqrt(f); }

        inline float Sin(float f) { return sgl_Maths_Sin(f); }
        inline float SinRange(float f, float min, float max) { return sgl_Maths_SinRange(f, min, max); }

        inline float Cos(float f) { return sgl_Maths_Cos(f); }
        inline float CosRange(float f, float min, float max) { return sgl_Maths_CosRange(f, min, max); }

        inline float Tan(float f) { return sgl_Maths_Tan(f); }

        inline float ASin(float f) { return sgl_Maths_ASin(f); }
        inline float ACos(float f) { return sgl_Maths_ACos(f); }
        inline float ATan(float f) { return sgl_Maths_ATan(f); }
        inline float ATan2(float y, float x) { return sgl_Maths_ATan2(y, x); }

        inline float Log(float f) { return sgl_Maths_Log(f); }

        inline float Floor(float f) { return sgl_Maths_Floor(f); }
        inline float Ceil(float f) { return sgl_Maths_Ceil(f); }
        inline float Round(float f) { return sgl_Maths_Round(f); }

        inline float Pow(float base, float exponent) { return sgl_Maths_Pow(base, exponent); }

        inline float LerpDT(float a, float b, float t, float dt) { return sgl_Maths_LerpDT(a, b, t, dt); }

        // Vec2
        inline float Dist(sgl_Vec2 a, sgl_Vec2 b) { return sgl_Maths_Vec2_Dist(a, b); }
        inline float Dist2(sgl_Vec2 a, sgl_Vec2 b) { return sgl_Maths_Vec2_Dist2(a, b); }
        inline float Angle(sgl_Vec2 a, sgl_Vec2 b) { return sgl_Maths_Vec2_Angle(a, b); }
        inline float Direction(sgl_Vec2 v) { return sgl_Maths_Vec2_Direction(v); }
        inline float Length(sgl_Vec2 v) { return sgl_Maths_Vec2_Length(v); }
        inline float Length2(sgl_Vec2 v) { return sgl_Maths_Vec2_Length2(v); }
        inline float Dot(sgl_Vec2 a, sgl_Vec2 b) { return sgl_Maths_Vec2_Dot(a, b); }
        inline float AspectRatio(sgl_Vec2 v) { return sgl_Maths_Vec2_AspectRatio(v); }
        inline sgl_Vec2 Floor(sgl_Vec2 v) { return sgl_Maths_Vec2_Floor(v); }
        inline sgl_Vec2 Normalise(sgl_Vec2 v) { return sgl_Maths_Vec2_Normalise(v); }

        // Vec2i
        inline float Dist(sgl_Vec2i a, sgl_Vec2i b) { return sgl_Maths_Vec2i_Dist(a, b); }
        inline float Dist2(sgl_Vec2i a, sgl_Vec2i b) { return sgl_Maths_Vec2i_Dist2(a, b); }
        inline float Angle(sgl_Vec2i a, sgl_Vec2i b) { return sgl_Maths_Vec2i_Angle(a, b); }
        inline float Direction(sgl_Vec2i v) { return sgl_Maths_Vec2i_Direction(v); }
        inline float Length(sgl_Vec2i v) { return sgl_Maths_Vec2i_Length(v); }
        inline float Length2(sgl_Vec2i v) { return sgl_Maths_Vec2i_Length2(v); }
        inline float Dot(sgl_Vec2i a, sgl_Vec2i b) { return sgl_Maths_Vec2i_Dot(a, b); }
        inline float AspectRatio(sgl_Vec2i v) { return sgl_Maths_Vec2i_AspectRatio(v); }

        // Vec3
        inline float Dist(sgl_Vec3 a, sgl_Vec3 b) { return sgl_Maths_Vec3_Dist(a, b); }
        inline float Dist2(sgl_Vec3 a, sgl_Vec3 b) { return sgl_Maths_Vec3_Dist2(a, b); }
        inline float Angle(sgl_Vec3 a, sgl_Vec3 b) { return sgl_Maths_Vec3_Angle(a, b); }
        inline float Length(sgl_Vec3 v) { return sgl_Maths_Vec3_Length(v); }
        inline float Length2(sgl_Vec3 v) { return sgl_Maths_Vec3_Length2(v); }
        inline float Dot(sgl_Vec3 a, sgl_Vec3 b) { return sgl_Maths_Vec3_Dot(a, b); }
        inline sgl_Vec3 Cross(sgl_Vec3 a, sgl_Vec3 b) { return sgl_Maths_Vec3_Cross(a, b); }
        inline sgl_Vec3 Normalise(sgl_Vec3 v) { return sgl_Maths_Vec3_Normalise(v); }

        // Vec3i
        inline float Dist(sgl_Vec3i a, sgl_Vec3i b) { return sgl_Maths_Vec3i_Dist(a, b); }
        inline float Dist2(sgl_Vec3i a, sgl_Vec3i b) { return sgl_Maths_Vec3i_Dist2(a, b); }
        inline float Angle(sgl_Vec3i a, sgl_Vec3i b) { return sgl_Maths_Vec3i_Angle(a, b); }
        inline float Length(sgl_Vec3i v) { return sgl_Maths_Vec3i_Length(v); }
        inline float Length2(sgl_Vec3i v) { return sgl_Maths_Vec3i_Length2(v); }
        inline float Dot(sgl_Vec3i a, sgl_Vec3i b) { return sgl_Maths_Vec3i_Dot(a, b); }
        inline sgl_Vec3i Cross(sgl_Vec3i a, sgl_Vec3i b) { return sgl_Maths_Vec3i_Cross(a, b); }

        // Vec4
        inline float Dist(sgl_Vec4 a, sgl_Vec4 b) { return sgl_Maths_Vec4_Dist(a, b); }
        inline float Dist2(sgl_Vec4 a, sgl_Vec4 b) { return sgl_Maths_Vec4_Dist2(a, b); }
        inline float Length(sgl_Vec4 v) { return sgl_Maths_Vec4_Length(v); }
        inline float Length2(sgl_Vec4 v) { return sgl_Maths_Vec4_Length2(v); }
        inline float Dot(sgl_Vec4 a, sgl_Vec4 b) { return sgl_Maths_Vec4_Dot(a, b); }
        inline sgl_Vec4 Normalise(sgl_Vec4 v) { return sgl_Maths_Vec4_Normalise(v); }

        // Mat4
        inline sgl_Mat4 Orthographic(sgl_Vec2i screenSize, float zoom) { return sgl_Maths_Mat4_Orthographic(screenSize, zoom); }
        inline sgl_Mat4 OrthographicGL(sgl_Vec2i screenSize, float zoom) { return sgl_Maths_Mat4_OrthographicGL(screenSize, zoom); }
        inline sgl_Mat4 Perspective(float fovY, float aspectRatio, float nearPlane, float farPlane) { return sgl_Maths_Mat4_Perspective(fovY, aspectRatio, nearPlane, farPlane); }
        inline sgl_Mat4 View(sgl_Vec3 position, float angle) { return sgl_Maths_Mat4_View(position, angle); }
        inline sgl_Mat4 LookAt(sgl_Vec3 eye, sgl_Vec3 target, sgl_Vec3 up) { return sgl_Maths_Mat4_LookAt(eye, target, up); }
        inline sgl_Mat4 Inverse(sgl_Mat4 mat) { return sgl_Maths_Mat4_Inverse(mat); }
        inline sgl_Mat4 Transpose(sgl_Mat4 mat) { return sgl_Maths_Mat4_Transpose(mat); }
        inline sgl_Mat4 Translation(sgl_Vec3 translation) { return sgl_Maths_Mat4_Translation(translation); }
        inline sgl_Mat4 Rotation(float angle, sgl_Vec3 axis) { return sgl_Maths_Mat4_Rotation(angle, axis); }
        inline sgl_Mat4 Scale(sgl_Vec3 scale) { return sgl_Maths_Mat4_Scale(scale); }
    }
}

#endif