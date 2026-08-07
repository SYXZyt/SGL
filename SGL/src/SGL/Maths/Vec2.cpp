#include "Vec2.h"
#include <SGL/Maths/Maths.h>


#define IMPL(t, v) t t::v = t##_##v; t##i t##i::v = t##i##_##v

IMPL(sgl_Vec2, Zero);
IMPL(sgl_Vec2, One);
IMPL(sgl_Vec2, Left);
IMPL(sgl_Vec2, Right);
IMPL(sgl_Vec2, Up);
IMPL(sgl_Vec2, Down);
IMPL(sgl_Vec2, Inf);
IMPL(sgl_Vec2, NegativeInf);

const sgl_Vec2 sgl_Vec2_Zero = {{{ 0.0f, 0.0f }}};
const sgl_Vec2 sgl_Vec2_One = {{{ 1.f, 1.f }}};
const sgl_Vec2 sgl_Vec2_Left = {{{ -1, 0.f }}};
const sgl_Vec2 sgl_Vec2_Right = {{{ 1.f, 0.f }}};
const sgl_Vec2 sgl_Vec2_Up = {{{ 0.f, 1.f }}};
const sgl_Vec2 sgl_Vec2_Down = {{{ 0.f, -1.f }}};
const sgl_Vec2 sgl_Vec2_Inf = {{{ sgl_Maths_INF, sgl_Maths_INF }}};
const sgl_Vec2 sgl_Vec2_NegativeInf = {{{ sgl_Maths_NEG_INF, sgl_Maths_NEG_INF }}};

sgl_Vec2 sgl_Vec2_New_Scalar(float scalar) {
    return {{{ scalar, scalar }}};
}

sgl_Vec2 sgl_Vec2_New_ScalarXY(float x, float y) {
    return {{{ x, y }}};
}

sgl_Vec2 sgl_Vec2_Add_Vec2(sgl_Vec2 a, sgl_Vec2 b) {
    return {{{ a.x + b.x, a.y + b.y }}};
}

sgl_Vec2 sgl_Vec2_Sub_Vec2(sgl_Vec2 a, sgl_Vec2 b) {
    return {{{ a.x - b.x, a.y - b.y }}};
}

sgl_Vec2 sgl_Vec2_Mul_Vec2(sgl_Vec2 a, sgl_Vec2 b) {
    return {{{ a.x * b.x, a.y * b.y }}};
}

sgl_Vec2 sgl_Vec2_Div_Vec2(sgl_Vec2 a, sgl_Vec2 b) {
    return {{{ a.x / b.x, a.y / b.y }}};
}

sgl_Vec2 sgl_Vec2_Mul_Scalar(sgl_Vec2 v, float scalar) {
    return {{{ v.x * scalar, v.y * scalar }}};
}

sgl_Vec2 sgl_Vec2_Div_Scalar(sgl_Vec2 v, float scalar) {
    return {{{ v.x / scalar, v.y / scalar }}};
}

sgl_Vec2 sgl_Vec2_Negate(sgl_Vec2 v) {
    return {{{ -v.x, -v.y }}};
}

bool sgl_Vec2_Compare(sgl_Vec2 a, sgl_Vec2 b) {
    return a.x == b.x && a.y == b.y;
}



const sgl_Vec2i sgl_Vec2i_Zero = {{{ 0, 0 }}};
const sgl_Vec2i sgl_Vec2i_One = {{{ 1, 1 }}};
const sgl_Vec2i sgl_Vec2i_Left = {{{ -1, 0 }}};
const sgl_Vec2i sgl_Vec2i_Right = {{{ 1, 0 }}};
const sgl_Vec2i sgl_Vec2i_Up = {{{ 0, 1 }}};
const sgl_Vec2i sgl_Vec2i_Down = {{{ 0, -1 }}};
const sgl_Vec2i sgl_Vec2i_Inf = {{{ (int)sgl_Maths_INF, (int)sgl_Maths_INF }}};
const sgl_Vec2i sgl_Vec2i_NegativeInf = {{{ (int)sgl_Maths_NEG_INF, (int)sgl_Maths_NEG_INF }}};

sgl_Vec2i sgl_Vec2i_New_Scalar(int scalar) {
    return {{{ scalar, scalar }}};
}

sgl_Vec2i sgl_Vec2i_New_ScalarXY(int x, int y) {
    return {{{ x, y }}};
}

sgl_Vec2i sgl_Vec2i_Add_Vec2i(sgl_Vec2i a, sgl_Vec2i b) {
    return {{{ a.x + b.x, a.y + b.y }}};
}

sgl_Vec2i sgl_Vec2i_Sub_Vec2i(sgl_Vec2i a, sgl_Vec2i b) {
    return {{{ a.x - b.x, a.y - b.y }}};
}

sgl_Vec2i sgl_Vec2i_Mul_Vec2i(sgl_Vec2i a, sgl_Vec2i b) {
    return {{{ a.x * b.x, a.y * b.y }}};
}

sgl_Vec2i sgl_Vec2i_Div_Vec2i(sgl_Vec2i a, sgl_Vec2i b) {
    return {{{ a.x / b.x, a.y / b.y }}};
}

sgl_Vec2i sgl_Vec2i_Mul_Scalar(sgl_Vec2i v, int scalar) {
    return {{{ v.x * scalar, v.y * scalar }}};
}

sgl_Vec2i sgl_Vec2i_Div_Scalar(sgl_Vec2i v, int scalar) {
    return {{{ v.x / scalar, v.y / scalar }}};
}

sgl_Vec2i sgl_Vec2i_Negate(sgl_Vec2i v) {
    return {{{ -v.x, -v.y }}};
}

bool sgl_Vec2i_Compare(sgl_Vec2i a, sgl_Vec2i b) {
    return a.x == b.x && a.y == b.y;
}