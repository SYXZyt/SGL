#include "Maths.h"
#include <numbers>
#include <limits>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

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

float sgl_Maths_Vec2_Dist(sgl_Vec2 a, sgl_Vec2 b) {
    return sgl_Maths_Sqrt(sgl_Maths_Vec2_Dist2(a, b));
}

float sgl_Maths_Vec2_Dist2(sgl_Vec2 a, sgl_Vec2 b)
{
    const float dx = b.x - a.x;
    const float dy = b.y - a.y;

    return sgl_Maths_Sq(dx) + sgl_Maths_Sq(dy);
}

float sgl_Maths_Vec2_Angle(sgl_Vec2 a, sgl_Vec2 b)
{
    const float dot = sgl_Maths_Vec2_Dot(a, b);
    const float lengths = sgl_Maths_Vec2_Length(a) * sgl_Maths_Vec2_Length(b);

    return sgl_Maths_ACos(dot / lengths);
}

float sgl_Maths_Vec2_Direction(sgl_Vec2 v) {
    return sgl_Maths_ATan2(v.y, v.x);
}

float sgl_Maths_Vec2_Length(sgl_Vec2 v) {
    return sgl_Maths_Sqrt(sgl_Maths_Vec2_Length2(v));
}

float sgl_Maths_Vec2_Length2(sgl_Vec2 v) {
    return sgl_Maths_Sq(v.x) + sgl_Maths_Sq(v.y);
}

float sgl_Maths_Vec2_Dot(sgl_Vec2 a, sgl_Vec2 b) {
    return a.x * b.x + a.y * b.y;
}

float sgl_Maths_Vec2_AspectRatio(sgl_Vec2 v)
{
    if (v.y == 0.f)
        return sgl_Maths_INF;

    return v.width / v.height;
}

sgl_Vec2 sgl_Maths_Vec2_Floor(sgl_Vec2 v)
{
    return { {{
        sgl_Maths_Floor(v.x),
        sgl_Maths_Floor(v.y)
    }} };
}

sgl_Vec2 sgl_Maths_Vec2_Normalise(sgl_Vec2 v)
{
    float length = sgl_Maths_Vec2_Length(v);
    if (length == 0)
        return sgl_Vec2_Zero;

    return sgl_Vec2_New_ScalarXY(v.x / length, v.y / length);
}

float sgl_Maths_Vec2i_Dist(sgl_Vec2i a, sgl_Vec2i b) {
    return sgl_Maths_Sqrt(sgl_Maths_Vec2i_Dist2(a, b));
}

float sgl_Maths_Vec2i_Dist2(sgl_Vec2i a, sgl_Vec2i b)
{
    const int dx = b.x - a.x;
    const int dy = b.y - a.y;

    return (float)(sgl_Maths_Sq(dx) + sgl_Maths_Sq(dy));
}

float sgl_Maths_Vec2i_Angle(sgl_Vec2i a, sgl_Vec2i b)
{
    const float dot = sgl_Maths_Vec2i_Dot(a, b);
    const float lengths = sgl_Maths_Vec2i_Length(a) * sgl_Maths_Vec2i_Length(b);

    return sgl_Maths_ACos(dot / lengths);
}

float sgl_Maths_Vec2i_Direction(sgl_Vec2i v) {
    return sgl_Maths_ATan2((float)v.y, (float)v.x);
}

float sgl_Maths_Vec2i_Length(sgl_Vec2i v) {
    return sgl_Maths_Sqrt(sgl_Maths_Vec2i_Length2(v));
}

float sgl_Maths_Vec2i_Length2(sgl_Vec2i v) {
    return (float)(sgl_Maths_Sq(v.x) + sgl_Maths_Sq(v.y));
}

float sgl_Maths_Vec2i_Dot(sgl_Vec2i a, sgl_Vec2i b) {
    return (float)a.x * b.x + a.y * b.y;
}

float sgl_Maths_Vec2i_AspectRatio(sgl_Vec2i v)
{
    if (v.y == 0.f)
        return sgl_Maths_INF;

    return (float)v.width / v.height;
}

static sgl_Mat4 FromGLM(const glm::mat4& glmMat) {
    return sgl_Mat4_New_Floats(glm::value_ptr(glmMat));
}

static glm::mat4 ToGLM(const sgl_Mat4& mat) {
    return glm::make_mat4(mat.values);
}

sgl_Mat4 sgl_Maths_Mat4_Orthographic(sgl_Vec2i screenSize, float zoom)
{
    float halfWidth = screenSize.width / 2.f * zoom;
    float halfHeight = screenSize.height / 2.f * zoom;

    glm::mat4 ortho = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, -1.f, 1.f);
    return FromGLM(ortho);
}

sgl_Mat4 sgl_Maths_Mat4_OrthographicGL(sgl_Vec2i screenSize, float zoom)
{
    float halfWidth = screenSize.width / 2.f * zoom;
    float halfHeight = screenSize.height / 2.f * zoom;

    glm::mat4 ortho = glm::ortho(-halfWidth, halfWidth, halfHeight, -halfHeight, -1.f, 1.f);
    return FromGLM(ortho);
}

sgl_Mat4 sgl_Maths_Mat4_View(sgl_Vec2 position, float angle)
{
    glm::mat4 translation = glm::translate(glm::mat4(1.f), glm::vec3(-position.x, -position.y, 0.f));

    float c = sgl_Maths_Cos(-angle);
    float s = sgl_Maths_Sin(-angle);

    glm::mat4 rotation =
    {
        { c, -s, 0, 0 },
        { s,  c, 0, 0 },
        { 0,  0, 1, 0 },
        { 0,  0, 0, 1 }
    };

    glm::mat4 view = rotation * translation;
    return FromGLM(view);
}

sgl_Mat4 sgl_Maths_Mat4_Inverse(sgl_Mat4 mat) {
    return FromGLM(glm::inverse(ToGLM(mat)));
}

sgl_Mat4 sgl_Maths_Mat4_Transpose(sgl_Mat4 mat) {
    return FromGLM(glm::transpose(ToGLM(mat)));
}

sgl_Mat4 sgl_Maths_Mat4_Translation(sgl_Vec3 translation)
{
    glm::vec3 glmTranslation(translation.x, translation.y, translation.z);
    return FromGLM(glm::translate(glm::mat4(1.f), glmTranslation));
}

sgl_Mat4 sgl_Maths_Mat4_Rotation(float angle, sgl_Vec3 axis)
{
    glm::vec3 glmAxis(axis.x, axis.y, axis.z);
    return FromGLM(glm::rotate(glm::mat4(1.f), angle, glmAxis));
}