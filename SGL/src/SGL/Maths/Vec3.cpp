#include "Vec3.h"
#include <SGL/Maths/Maths.h>

const sgl_Vec3 sgl_Vec3_Zero = { {{ 0.0f, 0.0f, 0.f }} };
const sgl_Vec3 sgl_Vec3_One = { {{ 1.f, 1.f, 0.f }} };
const sgl_Vec3 sgl_Vec3_Left = { {{ -1.f, 0.f, 0.f }} };
const sgl_Vec3 sgl_Vec3_Right = { {{ 1.f, 0.f, 0.f }} };
const sgl_Vec3 sgl_Vec3_Up = { {{ 0.f, 1.f, 0.f }} };
const sgl_Vec3 sgl_Vec3_Down = { {{ 0.f, -1.f, 0.f }} };
const sgl_Vec3 sgl_Vec3_Forward = { {{ 0.f, 0.f, 1.f }} };
const sgl_Vec3 sgl_Vec3_Backward = { {{ 0.f, 0.f, -1.f }} };
const sgl_Vec3 sgl_Vec3_Inf = { {{ sgl_Maths_INF, sgl_Maths_INF, sgl_Maths_INF }} };
const sgl_Vec3 sgl_Vec3_NegativeInf = { {{ sgl_Maths_NEG_INF, sgl_Maths_NEG_INF, sgl_Maths_NEG_INF }} };

sgl_Vec3 sgl_Vec3_New_Scalar(float scalar) {
    return { {{ scalar, scalar, scalar }} };
}

sgl_Vec3 sgl_Vec3_New_ScalarXYZ(float x, float y, float z) {
    return { {{ x, y, z }} };
}

sgl_Vec3 sgl_Vec3_Add_Vec3(sgl_Vec3 a, sgl_Vec3 b) {
    return { {{ a.x + b.x, a.y + b.y, a.z + b.z }} };
}

sgl_Vec3 sgl_Vec3_Sub_Vec3(sgl_Vec3 a, sgl_Vec3 b) {
    return { {{ a.x - b.x, a.y - b.y, a.z - b.z }} };
}

sgl_Vec3 sgl_Vec3_Mul_Vec3(sgl_Vec3 a, sgl_Vec3 b) {
    return { {{ a.x * b.x, a.y * b.y, a.z * b.z }} };
}

sgl_Vec3 sgl_Vec3_Div_Vec3(sgl_Vec3 a, sgl_Vec3 b) {
    return { {{ a.x / b.x, a.y / b.y, a.z / b.z }} };
}

sgl_Vec3 sgl_Vec3_Mul_Scalar(sgl_Vec3 v, float scalar) {
    return { {{ v.x * scalar, v.y * scalar, v.z * scalar }} };
}

sgl_Vec3 sgl_Vec3_Div_Scalar(sgl_Vec3 v, float scalar) {
    return { {{ v.x / scalar, v.y / scalar, v.z / scalar }} };
}

sgl_Vec3 sgl_Vec3_Negate(sgl_Vec3 v) {
    return { {{ -v.x, -v.y, -v.z }} };
}

bool sgl_Vec3_Compare(sgl_Vec3 a, sgl_Vec3 b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}



const sgl_Vec3i sgl_Vec3i_Zero = { {{ 0, 0, 0 }} };
const sgl_Vec3i sgl_Vec3i_One = { {{ 1, 1, 0 }} };
const sgl_Vec3i sgl_Vec3i_Left = { {{ -1, 0, 0 }} };
const sgl_Vec3i sgl_Vec3i_Right = { {{ 1, 0, 0 }} };
const sgl_Vec3i sgl_Vec3i_Up = { {{ 0, 1, 0 }} };
const sgl_Vec3i sgl_Vec3i_Down = { {{ 0, -1, 0 }} };
const sgl_Vec3i sgl_Vec3i_Forward = { {{ 0, 0, 1 }} };
const sgl_Vec3i sgl_Vec3i_Backward = { {{ 0, 0, -1 }} };
const sgl_Vec3i sgl_Vec3i_Inf = { {{ (int)sgl_Maths_INF, (int)sgl_Maths_INF, (int)sgl_Maths_INF }} };
const sgl_Vec3i sgl_Vec3i_NegativeInf = { {{ (int)sgl_Maths_NEG_INF, (int)sgl_Maths_NEG_INF, (int)sgl_Maths_NEG_INF }} };

sgl_Vec3i sgl_Vec3i_New_Scalar(int scalar) {
    return { {{ scalar, scalar, scalar }} };
}

sgl_Vec3i sgl_Vec3i_New_ScalarXYZ(int x, int y, int z) {
    return { {{ x, y, z }} };
}

sgl_Vec3i sgl_Vec3i_Add_Vec3i(sgl_Vec3i a, sgl_Vec3i b) {
    return { {{ a.x + b.x, a.y + b.y, a.z + b.z }} };
}

sgl_Vec3i sgl_Vec3i_Sub_Vec3i(sgl_Vec3i a, sgl_Vec3i b) {
    return { {{ a.x - b.x, a.y - b.y, a.z - b.z }} };
}

sgl_Vec3i sgl_Vec3i_Mul_Vec3i(sgl_Vec3i a, sgl_Vec3i b) {
    return { {{ a.x * b.x, a.y * b.y, a.z * b.z }} };
}

sgl_Vec3i sgl_Vec3i_Div_Vec3i(sgl_Vec3i a, sgl_Vec3i b) {
    return { {{ a.x / b.x, a.y / b.y, a.z / b.z }} };
}

sgl_Vec3i sgl_Vec3i_Mul_Scalar(sgl_Vec3i v, int scalar) {
    return { {{ v.x * scalar, v.y * scalar, v.z * scalar }} };
}

sgl_Vec3i sgl_Vec3i_Div_Scalar(sgl_Vec3i v, int scalar) {
    return { {{ v.x / scalar, v.y / scalar, v.z / scalar }} };
}

sgl_Vec3i sgl_Vec3i_Negate(sgl_Vec3i v) {
    return { {{ -v.x, -v.y, -v.z }} };
}

bool sgl_Vec3i_Compare(sgl_Vec3i a, sgl_Vec3i b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}