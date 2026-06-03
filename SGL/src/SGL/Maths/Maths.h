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

#define sgl_Maths_DEG2RAD PI / 180.f
#define sgl_Maths_RAD2DEG 180.f / PI

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
#define sgl_Maths_Max(a, b) ((a) > (b) ? (b) : (a))
#define sgl_Maths_Clamp(value, min, max) (sgl_Maths_Max((min), sgl_Maths_Min((max), (value))))

#define sgl_Maths_Abs(f) (f) < 0 ? -(f) : (f)
#define sgl_Maths_Copysign(f, sign) ((sign) < 0) ? -sgl_Maths_Abs((f)) : sgl_Maths_Abs((f))

#define sgl_Maths_Trunc(f) ((f) < 0 ? (float)(int)((f) + 1.f) : (float)(int)((f)))

#define sgl_Maths_Rad(deg) ((deg) * sgl_Maths_DEG2RAD)
#define sgl_Maths_Deg(rad) ((deg) * sgl_Maths_RAD2DEG)

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

#pragma region Mat4
SGL_API extern sgl_Mat4 sgl_Maths_Mat4_Orthographic(sgl_Vec2i screenSize, float zoom);
SGL_API extern sgl_Mat4 sgl_Maths_Mat4_OrthographicGL(sgl_Vec2i screenSize, float zoom);

SGL_API extern sgl_Mat4 sgl_Maths_Mat4_View(sgl_Vec2 position, float angle);

SGL_API extern sgl_Mat4 sgl_Maths_Mat4_Inverse(sgl_Mat4 mat);
SGL_API extern sgl_Mat4 sgl_Maths_Mat4_Transpose(sgl_Mat4 mat);

SGL_API extern sgl_Mat4 sgl_Maths_Mat4_Translation(sgl_Vec3 translation);
SGL_API extern sgl_Mat4 sgl_Maths_Mat4_Rotation(float angle, sgl_Vec3 axis);
#pragma endregion

SGL_END