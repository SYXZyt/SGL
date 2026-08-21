#pragma once
#include <SGL/SGL.h>

SGL_BEGIN

#pragma region Vec2

typedef struct sgl_Vec2 sgl_sealed
{
    union
    {
        struct { float x, y; };
        struct { float u, v; };
        struct { float width, height; };
        float values[2];
    };

#ifdef __cplusplus
    SGL_API static sgl_Vec2 Zero;
    SGL_API static sgl_Vec2 One;
    SGL_API static sgl_Vec2 Left;
    SGL_API static sgl_Vec2 Right;
    SGL_API static sgl_Vec2 Up;
    SGL_API static sgl_Vec2 Down;
    SGL_API static sgl_Vec2 Inf;
    SGL_API static sgl_Vec2 NegativeInf;
#endif

    // We can't use a constructor since it causes compile errors
    // well it does if you are using this in a c++ project
#ifdef  __cplusplus
    static sgl_Vec2 Make();
    static sgl_Vec2 Make(float scalar);
    static sgl_Vec2 Make(float x, float y);
#endif

} sgl_Vec2;

SGL_API extern const sgl_Vec2 sgl_Vec2_Zero;
SGL_API extern const sgl_Vec2 sgl_Vec2_One;
SGL_API extern const sgl_Vec2 sgl_Vec2_Left;
SGL_API extern const sgl_Vec2 sgl_Vec2_Right;
SGL_API extern const sgl_Vec2 sgl_Vec2_Up;
SGL_API extern const sgl_Vec2 sgl_Vec2_Down;
SGL_API extern const sgl_Vec2 sgl_Vec2_Inf;
SGL_API extern const sgl_Vec2 sgl_Vec2_NegativeInf;

SGL_API extern sgl_Vec2 sgl_Vec2_New_Scalar(float scalar);
SGL_API extern sgl_Vec2 sgl_Vec2_New_ScalarXY(float x, float y);

SGL_API extern sgl_Vec2 sgl_Vec2_Add_Vec2(sgl_Vec2 a, sgl_Vec2 b);
SGL_API extern sgl_Vec2 sgl_Vec2_Sub_Vec2(sgl_Vec2 a, sgl_Vec2 b);
SGL_API extern sgl_Vec2 sgl_Vec2_Mul_Vec2(sgl_Vec2 a, sgl_Vec2 b);
SGL_API extern sgl_Vec2 sgl_Vec2_Div_Vec2(sgl_Vec2 a, sgl_Vec2 b);

SGL_API extern sgl_Vec2 sgl_Vec2_Mul_Scalar(sgl_Vec2 v, float scalar);
SGL_API extern sgl_Vec2 sgl_Vec2_Div_Scalar(sgl_Vec2 v, float scalar);

SGL_API extern sgl_Vec2 sgl_Vec2_Negate(sgl_Vec2 v);

SGL_API extern bool sgl_Vec2_Compare(sgl_Vec2 a, sgl_Vec2 b);

#pragma endregion

#pragma region Vec2i

typedef struct sgl_Vec2i
{
    union
    {
        struct { int x, y; };
        struct { int u, v; };
        struct { int width, height; };
        int values[2];
    };

#ifdef __cplusplus
    SGL_API static sgl_Vec2i Zero;
    SGL_API static sgl_Vec2i One;
    SGL_API static sgl_Vec2i Left;
    SGL_API static sgl_Vec2i Right;
    SGL_API static sgl_Vec2i Up;
    SGL_API static sgl_Vec2i Down;
    SGL_API static sgl_Vec2i Inf;
    SGL_API static sgl_Vec2i NegativeInf;
#endif
} sgl_Vec2i;

SGL_API extern const sgl_Vec2i sgl_Vec2i_Zero;
SGL_API extern const sgl_Vec2i sgl_Vec2i_One;
SGL_API extern const sgl_Vec2i sgl_Vec2i_Left;
SGL_API extern const sgl_Vec2i sgl_Vec2i_Right;
SGL_API extern const sgl_Vec2i sgl_Vec2i_Up;
SGL_API extern const sgl_Vec2i sgl_Vec2i_Down;
SGL_API extern const sgl_Vec2i sgl_Vec2i_Inf;
SGL_API extern const sgl_Vec2i sgl_Vec2i_NegativeInf;

SGL_API extern sgl_Vec2i sgl_Vec2i_New_Scalar(int scalar);
SGL_API extern sgl_Vec2i sgl_Vec2i_New_ScalarXY(int x, int y);

SGL_API extern sgl_Vec2i sgl_Vec2i_Add_Vec2i(sgl_Vec2i a, sgl_Vec2i b);
SGL_API extern sgl_Vec2i sgl_Vec2i_Sub_Vec2i(sgl_Vec2i a, sgl_Vec2i b);
SGL_API extern sgl_Vec2i sgl_Vec2i_Mul_Vec2i(sgl_Vec2i a, sgl_Vec2i b);
SGL_API extern sgl_Vec2i sgl_Vec2i_Div_Vec2i(sgl_Vec2i a, sgl_Vec2i b);

SGL_API extern sgl_Vec2i sgl_Vec2i_Mul_Scalar(sgl_Vec2i v, int scalar);
SGL_API extern sgl_Vec2i sgl_Vec2i_Div_Scalar(sgl_Vec2i v, int scalar);

SGL_API extern sgl_Vec2i sgl_Vec2i_Negate(sgl_Vec2i v);

SGL_API extern bool sgl_Vec2i_Compare(sgl_Vec2i a, sgl_Vec2i b);

#pragma endregion

SGL_END

// If using C++, it is nice to have some operators
#ifdef __cplusplus

inline sgl_Vec2 operator+(sgl_Vec2 a, sgl_Vec2 b) {
    return sgl_Vec2_Add_Vec2(a, b);
}

inline sgl_Vec2 operator-(sgl_Vec2 a, sgl_Vec2 b) {
    return sgl_Vec2_Sub_Vec2(a, b);
}

inline sgl_Vec2 operator*(sgl_Vec2 a, sgl_Vec2 b) {
    return sgl_Vec2_Mul_Vec2(a, b);
}

inline sgl_Vec2 operator/(sgl_Vec2 a, sgl_Vec2 b) {
    return sgl_Vec2_Div_Vec2(a, b);
}

inline sgl_Vec2 operator*(sgl_Vec2 a, float b) {
    return sgl_Vec2_Mul_Scalar(a, b);
}

inline sgl_Vec2 operator/(sgl_Vec2 a, float b) {
    return sgl_Vec2_Div_Scalar(a, b);
}

inline sgl_Vec2 operator-(sgl_Vec2 v) {
    return sgl_Vec2_Negate(v);
}

inline bool operator==(sgl_Vec2 a, sgl_Vec2 b) {
    return sgl_Vec2_Compare(a, b);
}

inline bool operator!=(sgl_Vec2 a, sgl_Vec2 b) {
    return !(a == b);
}

inline sgl_Vec2& operator+=(sgl_Vec2& self, sgl_Vec2 other)
{
    self.x += other.x;
    self.y += other.y;

    return self;
}

inline sgl_Vec2& operator+=(sgl_Vec2& self, float scalar)
{
    self.x += scalar;
    self.y += scalar;

    return self;
}

inline sgl_Vec2& operator-=(sgl_Vec2& self, sgl_Vec2 other)
{
    self.x -= other.x;
    self.y -= other.y;

    return self;
}

inline sgl_Vec2& operator-=(sgl_Vec2& self, float scalar)
{
    self.x -= scalar;
    self.y -= scalar;

    return self;
}

inline sgl_Vec2& operator*=(sgl_Vec2& self, sgl_Vec2 other)
{
    self.x *= other.x;
    self.y *= other.y;

    return self;
}

inline sgl_Vec2& operator*=(sgl_Vec2& self, float scalar)
{
    self.x *= scalar;
    self.y *= scalar;

    return self;
}

inline sgl_Vec2i operator+(sgl_Vec2i a, sgl_Vec2i b) {
    return sgl_Vec2i_Add_Vec2i(a, b);
}

inline sgl_Vec2i operator-(sgl_Vec2i a, sgl_Vec2i b) {
    return sgl_Vec2i_Sub_Vec2i(a, b);
}

inline sgl_Vec2i operator*(sgl_Vec2i a, sgl_Vec2i b) {
    return sgl_Vec2i_Mul_Vec2i(a, b);
}

inline sgl_Vec2i operator/(sgl_Vec2i a, sgl_Vec2i b) {
    return sgl_Vec2i_Div_Vec2i(a, b);
}

inline sgl_Vec2i operator*(sgl_Vec2i a, int b) {
    return sgl_Vec2i_Mul_Scalar(a, b);
}

inline sgl_Vec2i operator/(sgl_Vec2i a, int b) {
    return sgl_Vec2i_Div_Scalar(a, b);
}

inline sgl_Vec2i operator-(sgl_Vec2i v) {
    return sgl_Vec2i_Negate(v);
}

inline bool operator==(sgl_Vec2i a, sgl_Vec2i b) {
    return sgl_Vec2i_Compare(a, b);
}

inline bool operator!=(sgl_Vec2i a, sgl_Vec2i b) {
    return !(a == b);
}

inline sgl_Vec2i& operator+=(sgl_Vec2i& self, sgl_Vec2i other)
{
    self.x += other.x;
    self.y += other.y;

    return self;
}

inline sgl_Vec2i& operator+=(sgl_Vec2i& self, int scalar)
{
    self.x += scalar;
    self.y += scalar;

    return self;
}

inline sgl_Vec2i& operator-=(sgl_Vec2i& self, sgl_Vec2i other)
{
    self.x -= other.x;
    self.y -= other.y;

    return self;
}

inline sgl_Vec2i& operator-=(sgl_Vec2i& self, int scalar)
{
    self.x -= scalar;
    self.y -= scalar;

    return self;
}

inline sgl_Vec2i& operator*=(sgl_Vec2i& self, sgl_Vec2i other)
{
    self.x *= other.x;
    self.y *= other.y;

    return self;
}

inline sgl_Vec2i& operator*=(sgl_Vec2i& self, int scalar)
{
    self.x *= scalar;
    self.y *= scalar;

    return self;
}

#endif