#include "Maths.h"
#include <numbers>
#include <limits>
#include <cmath>

const float sgl_Maths_PI = std::numbers::pi_v<float>;
const float sgl_Maths_HALF_PI = sgl_Maths_PI / 2.f;
const float sgl_Maths_TWO_PI = sgl_Maths_PI * 2.f;

const float sgl_Maths_EPS = std::numeric_limits<float>::epsilon();
const float sgl_Maths_INF = std::numeric_limits<float>::infinity();
const float sgl_Maths_NAN = std::numeric_limits<float>::quiet_NaN();

float sgl_Maths_Sqrt(float f) {
    return std::sqrtf(f);
}

float sgl_Maths_Sin(float f) {
    return std::sinf(f);
}

float sgl_Maths_SinRange(float f, float min, float max)
{
    float amplitude = (max - min) * 0.5f;
    float mid = (max + min) * 0.5f;
    return sgl_Maths_Sin(f) * amplitude + mid;
}

float sgl_Maths_Cos(float f) {
    return std::cosf(f);
}

float sgl_Maths_CosRange(float f, float min, float max)
{
    float amplitude = (max - min) * 0.5f;
    float mid = (max + min) * 0.5f;
    return sgl_Maths_Cos(f) * amplitude + mid;
}

float sgl_Maths_Tan(float f) {
    return std::tanf(f);
}

float sgl_Maths_ASin(float f) {
    return std::asinf(f);
}

float sgl_Maths_ACos(float f) {
    return std::acosf(f);
}

float sgl_Maths_ATan(float f) {
    return std::atanf(f);
}

float sgl_Maths_ATan2(float y, float x) {
    return std::atan2f(y, x);
}

float sgl_Maths_Log(float f) {
    return std::logf(f);
}

float sgl_Maths_Floor(float f) {
    return std::floorf(f);
}

float sgl_Maths_Ceil(float f) {
    return std::ceilf(f);
}

float sgl_Maths_Round(float f) {
    return std::roundf(f);
}

float sgl_Maths_Pow(float base, float exponent) {
    return std::powf(base, exponent);
}

float sgl_Maths_LerpDT(float a, float b, float t, float dt) {
    return sgl_Maths_Lerp(a, b, 1 - sgl_Maths_Pow(t, dt));
}

float sgl_Maths_Dist(sgl_Vec2 a, sgl_Vec2 b) {
    return sgl_Maths_Sqrt(sgl_Maths_Dist2(a, b));
}

float sgl_Maths_Dist2(sgl_Vec2 a, sgl_Vec2 b)
{
    const float dx = b.x - a.x;
    const float dy = b.y - a.y;

    return sgl_Maths_Sq(dx) + sgl_Maths_Sq(dy);
}

float sgl_Maths_Angle(sgl_Vec2 a, sgl_Vec2 b)
{
    const float dot = sgl_Maths_Dot(a, b);
    const float lengths = sgl_Maths_Length(a) * sgl_Maths_Length(b);

    return sgl_Maths_ACos(dot / lengths);
}

float sgl_Maths_Direction(sgl_Vec2 v) {
    return sgl_Maths_ATan2(v.y, v.x);
}

float sgl_Maths_Length(sgl_Vec2 v) {
    return sgl_Maths_Sqrt(sgl_Maths_Length2(v));
}

float sgl_Maths_Length2(sgl_Vec2 v) {
    return sgl_Maths_Sq(v.x) + sgl_Maths_Sq(v.y);
}

float sgl_Maths_Dot(sgl_Vec2 a, sgl_Vec2 b) {
    return a.x * b.x + a.y * b.y;
}

float sgl_Maths_AspectRatio(sgl_Vec2 v)
{
    if (v.y == 0.f)
        return sgl_Maths_INF;

    return v.width / v.height;
}

sgl_Vec2 sgl_Maths_Floor_Vec2(sgl_Vec2 v)
{
    return {
        sgl_Maths_Floor(v.x),
        sgl_Maths_Floor(v.y)
    };
}

float sgl_Maths_Dist_i(sgl_Vec2i a, sgl_Vec2i b) {
    return sgl_Maths_Sqrt(sgl_Maths_Dist2_i(a, b));
}

float sgl_Maths_Dist2_i(sgl_Vec2i a, sgl_Vec2i b)
{
    const float dx = b.x - a.x;
    const float dy = b.y - a.y;

    return sgl_Maths_Sq(dx) + sgl_Maths_Sq(dy);
}

float sgl_Maths_Angle_i(sgl_Vec2i a, sgl_Vec2i b)
{
    const float dot = sgl_Maths_Dot_i(a, b);
    const float lengths = sgl_Maths_Length_i(a) * sgl_Maths_Length_i(b);

    return sgl_Maths_ACos(dot / lengths);
}

float sgl_Maths_Direction_i(sgl_Vec2i v) {
    return sgl_Maths_ATan2(v.y, v.x);
}

float sgl_Maths_Length_i(sgl_Vec2i v) {
    return sgl_Maths_Sqrt(sgl_Maths_Length2_i(v));
}

float sgl_Maths_Length2_i(sgl_Vec2i v) {
    return sgl_Maths_Sq(v.x) + sgl_Maths_Sq(v.y);
}

float sgl_Maths_Dot_i(sgl_Vec2i a, sgl_Vec2i b) {
    return a.x * b.x + a.y * b.y;
}

float sgl_Maths_AspectRatio_i(sgl_Vec2i v)
{
    if (v.y == 0.f)
        return sgl_Maths_INF;

    return v.width / v.height;
}