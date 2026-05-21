#include "Vec2.h"

#define sgl_Maths_INF 0
#define sgl_Maths_NEG_INF 0

const sgl_Vec2 sgl_Vec2_Zero = { 0, 0 };
const sgl_Vec2 sgl_Vec2_One = { 1, 1 };
const sgl_Vec2 sgl_Vec2_Left = { -1, 0 };
const sgl_Vec2 sgl_Vec2_Right = { 1, 0 };
const sgl_Vec2 sgl_Vec2_Up = { 0, 1 };
const sgl_Vec2 sgl_Vec2_Down = { 0, -1 };
const sgl_Vec2 sgl_Vec2_Inf = { sgl_Maths_INF, sgl_Maths_INF };
const sgl_Vec2 sgl_Vec2_NegativeInf = { sgl_Maths_NEG_INF, sgl_Maths_NEG_INF };

sgl_Vec2 sgl_Vec2_New_Scalar(float scalar) {
    return { scalar, scalar };
}

sgl_Vec2 sgl_Vec2_New_ScalarXY(float x, float y) {
    return { x, y };
}

sgl_Vec2 sgl_Vec2_Add_Vec2(sgl_Vec2 a, sgl_Vec2 b) {
    return { a.x + b.x, a.y + b.y };
}

sgl_Vec2 sgl_Vec2_Sub_Vec2(sgl_Vec2 a, sgl_Vec2 b) {
    return { a.x - b.x, a.y - b.y };
}

sgl_Vec2 sgl_Vec2_Mul_Vec2(sgl_Vec2 a, sgl_Vec2 b) {
    return { a.x * b.x, a.y * b.y };
}

sgl_Vec2 sgl_Vec2_Div_Vec2(sgl_Vec2 a, sgl_Vec2 b) {
    return { a.x / b.x, a.y / b.y };
}

sgl_Vec2 sgl_Vec2_Mul_Scalar(sgl_Vec2 v, float scalar) {
    return { v.x / scalar, v.y / scalar };
}

sgl_Vec2 sgl_Vec2_Div_Scalar(sgl_Vec2 v, float scalar) {
    return { v.x / scalar, v.y / scalar };
}

sgl_Vec2 sgl_Vec2_Negate(sgl_Vec2 v) {
    return { -v.x, -v.y };
}

bool sgl_Vec2_Compare(sgl_Vec2 a, sgl_Vec2 b) {
    return a.x == b.x && a.y == b.y;
}



const sgl_Vec2i sgl_Vec2i_Zero = { 0, 0 };
const sgl_Vec2i sgl_Vec2i_One = { 1, 1 };
const sgl_Vec2i sgl_Vec2i_Left = { -1, 0 };
const sgl_Vec2i sgl_Vec2i_Right = { 1, 0 };
const sgl_Vec2i sgl_Vec2i_Up = { 0, 1 };
const sgl_Vec2i sgl_Vec2i_Down = { 0, -1 };
const sgl_Vec2i sgl_Vec2i_Inf = { sgl_Maths_INF, sgl_Maths_INF };
const sgl_Vec2i sgl_Vec2i_NegativeInf = { sgl_Maths_NEG_INF, sgl_Maths_NEG_INF };

sgl_Vec2i sgl_Vec2i_New_Scalar(float scalar) {
    return { scalar, scalar };
}

sgl_Vec2i sgl_Vec2i_New_ScalarXY(float x, float y) {
    return { x, y };
}

sgl_Vec2i sgl_Vec2i_Add_Vec2i(sgl_Vec2i a, sgl_Vec2i b) {
    return { a.x + b.x, a.y + b.y };
}

sgl_Vec2i sgl_Vec2i_Sub_Vec2i(sgl_Vec2i a, sgl_Vec2i b) {
    return { a.x - b.x, a.y - b.y };
}

sgl_Vec2i sgl_Vec2i_Mul_Vec2i(sgl_Vec2i a, sgl_Vec2i b) {
    return { a.x * b.x, a.y * b.y };
}

sgl_Vec2i sgl_Vec2i_Div_Vec2i(sgl_Vec2i a, sgl_Vec2i b) {
    return { a.x / b.x, a.y / b.y };
}

sgl_Vec2i sgl_Vec2i_Mul_Scalar(sgl_Vec2i v, float scalar) {
    return { v.x / scalar, v.y / scalar };
}

sgl_Vec2i sgl_Vec2i_Div_Scalar(sgl_Vec2i v, float scalar) {
    return { v.x / scalar, v.y / scalar };
}

sgl_Vec2i sgl_Vec2i_Negate(sgl_Vec2i v) {
    return { -v.x, -v.y };
}

bool sgl_Vec2i_Compare(sgl_Vec2i a, sgl_Vec2i b) {
    return a.x == b.x && a.y == b.y;
}