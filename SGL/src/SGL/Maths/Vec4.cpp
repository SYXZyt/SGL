#include "Vec4.h"
#include <SGL/Maths/Maths.h>

const sgl_Vec4 sgl_Vec4_Zero = { {{ 0.0f, 0.0f, 0.f, 0.f }} };
const sgl_Vec4 sgl_Vec4_One = { {{ 1.f, 1.f, 1.f, 1.f }} };
const sgl_Vec4 sgl_Vec4_Left = { {{ -1.f, 0.f, 0.f, 0.f }} };
const sgl_Vec4 sgl_Vec4_Right = { {{ 1.f, 0.f, 0.f, 0.f }} };
const sgl_Vec4 sgl_Vec4_Up = { {{ 0.f, 1.f, 0.f, 0.f }} };
const sgl_Vec4 sgl_Vec4_Down = { {{ 0.f, -1.f, 0.f, 0.f }} };
const sgl_Vec4 sgl_Vec4_Forward = { {{ 0.f, 0.f, 1.f, 0.f }} };
const sgl_Vec4 sgl_Vec4_Backward = { {{ 0.f, 0.f, -1.f, 0.f }} };
const sgl_Vec4 sgl_Vec4_Inf = { {{ sgl_Maths_INF, sgl_Maths_INF, sgl_Maths_INF, sgl_Maths_INF }} };
const sgl_Vec4 sgl_Vec4_NegativeInf = { {{ sgl_Maths_NEG_INF, sgl_Maths_NEG_INF, sgl_Maths_NEG_INF, sgl_Maths_NEG_INF }} };

sgl_Vec4 sgl_Vec4_New_Scalar(float scalar) {
    return { {{ scalar, scalar, scalar, scalar }} };
}

sgl_Vec4 sgl_Vec4_New_ScalarXYZW(float x, float y, float z, float w) {
    return { {{ x, y, z, w }} };
}

sgl_Vec4 sgl_Vec4_Add_Vec4(sgl_Vec4 a, sgl_Vec4 b) {
    return { {{ a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w }} };
}

sgl_Vec4 sgl_Vec4_Sub_Vec4(sgl_Vec4 a, sgl_Vec4 b) {
    return { {{ a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w }} };
}

sgl_Vec4 sgl_Vec4_Mul_Vec4(sgl_Vec4 a, sgl_Vec4 b) {
    return { {{ a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w }} };
}

sgl_Vec4 sgl_Vec4_Div_Vec4(sgl_Vec4 a, sgl_Vec4 b) {
    return { {{ a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w }} };
}

sgl_Vec4 sgl_Vec4_Mul_Scalar(sgl_Vec4 v, float scalar) {
    return { {{ v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar }} };
}

sgl_Vec4 sgl_Vec4_Div_Scalar(sgl_Vec4 v, float scalar) {
    return { {{ v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar }} };
}

sgl_Vec4 sgl_Vec4_Negate(sgl_Vec4 v) {
    return { {{ -v.x, -v.y, -v.z, -v.w }} };
}

bool sgl_Vec4_Compare(sgl_Vec4 a, sgl_Vec4 b) {
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}