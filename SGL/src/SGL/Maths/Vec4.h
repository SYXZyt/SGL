#pragma once
#include <SGL/SGL.h>

SGL_BEGIN

typedef struct sgl_Vec4 sgl_sealed
{
    union
    {
        struct { float x, y, z, w; };
        struct { float r, g, b, a; };
        float values[4];
    };

#ifdef __cplusplus
    SGL_API static sgl_Vec4 Zero;
    SGL_API static sgl_Vec4 One;
    SGL_API static sgl_Vec4 Left;
    SGL_API static sgl_Vec4 Right;
    SGL_API static sgl_Vec4 Up;
    SGL_API static sgl_Vec4 Down;
    SGL_API static sgl_Vec4 Forward;
    SGL_API static sgl_Vec4 Backward;
    SGL_API static sgl_Vec4 Inf;
    SGL_API static sgl_Vec4 NegativeInf;
#endif
} sgl_Vec4;

SGL_API extern const sgl_Vec4 sgl_Vec4_Zero;
SGL_API extern const sgl_Vec4 sgl_Vec4_One;
SGL_API extern const sgl_Vec4 sgl_Vec4_Left;
SGL_API extern const sgl_Vec4 sgl_Vec4_Right;
SGL_API extern const sgl_Vec4 sgl_Vec4_Up;
SGL_API extern const sgl_Vec4 sgl_Vec4_Down;
SGL_API extern const sgl_Vec4 sgl_Vec4_Forward;
SGL_API extern const sgl_Vec4 sgl_Vec4_Backward;
SGL_API extern const sgl_Vec4 sgl_Vec4_Inf;
SGL_API extern const sgl_Vec4 sgl_Vec4_NegativeInf;

SGL_API extern sgl_Vec4 sgl_Vec4_New_Scalar(float scalar);
SGL_API extern sgl_Vec4 sgl_Vec4_New_ScalarXYZW(float x, float y, float z, float w);

SGL_API extern sgl_Vec4 sgl_Vec4_Add_Vec4(sgl_Vec4 a, sgl_Vec4 b);
SGL_API extern sgl_Vec4 sgl_Vec4_Sub_Vec4(sgl_Vec4 a, sgl_Vec4 b);
SGL_API extern sgl_Vec4 sgl_Vec4_Mul_Vec4(sgl_Vec4 a, sgl_Vec4 b);
SGL_API extern sgl_Vec4 sgl_Vec4_Div_Vec4(sgl_Vec4 a, sgl_Vec4 b);

SGL_API extern sgl_Vec4 sgl_Vec4_Mul_Scalar(sgl_Vec4 v, float scalar);
SGL_API extern sgl_Vec4 sgl_Vec4_Div_Scalar(sgl_Vec4 v, float scalar);

SGL_API extern sgl_Vec4 sgl_Vec4_Negate(sgl_Vec4 v);

SGL_API extern bool sgl_Vec4_Compare(sgl_Vec4 a, sgl_Vec4 b);

SGL_END

// If using C++, it is nice to have some operators
#ifdef __cplusplus

inline sgl_Vec4 operator+(sgl_Vec4 a, sgl_Vec4 b) {
    return sgl_Vec4_Add_Vec4(a, b);
}

inline sgl_Vec4 operator-(sgl_Vec4 a, sgl_Vec4 b) {
    return sgl_Vec4_Sub_Vec4(a, b);
}

inline sgl_Vec4 operator*(sgl_Vec4 a, sgl_Vec4 b) {
    return sgl_Vec4_Mul_Vec4(a, b);
}

inline sgl_Vec4 operator/(sgl_Vec4 a, sgl_Vec4 b) {
    return sgl_Vec4_Div_Vec4(a, b);
}

inline sgl_Vec4 operator*(sgl_Vec4 a, float b) {
    return sgl_Vec4_Mul_Scalar(a, b);
}

inline sgl_Vec4 operator/(sgl_Vec4 a, float b) {
    return sgl_Vec4_Div_Scalar(a, b);
}

inline sgl_Vec4 operator-(sgl_Vec4 v) {
    return sgl_Vec4_Negate(v);
}

inline bool operator==(sgl_Vec4 a, sgl_Vec4 b) {
    return sgl_Vec4_Compare(a, b);
}

inline bool operator!=(sgl_Vec4 a, sgl_Vec4 b) {
    return !(a == b);
}

#endif