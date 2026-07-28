#pragma once
#include <SGL/SGL.h>
#include <SGL/Maths/Vec4.h>

SGL_BEGIN

typedef struct sgl_Mat4 sgl_sealed
{
    union
    {
        float values[16];
        float column[4][4];
    };
} sgl_Mat4;

SGL_API extern const sgl_Mat4 sgl_Mat4_Identity;

SGL_API extern sgl_Mat4 sgl_Mat4_New();
SGL_API extern sgl_Mat4 sgl_Mat4_New_Diagonal(float diagonal);
SGL_API extern sgl_Mat4 sgl_Mat4_New_Floats(const float* values);

SGL_API extern sgl_Mat4 sgl_Mat4_Mul_Mat4(sgl_Mat4 a, sgl_Mat4 b);
SGL_API extern sgl_Vec4 sgl_Mat4_Mul_Vec4(sgl_Mat4 mat, sgl_Vec4 v);

SGL_API extern bool sgl_Mat4_Compare(sgl_Mat4 a, sgl_Mat4 b);

SGL_END

// If using C++, it is nice to have some operators
#ifdef __cplusplus

inline sgl_Mat4 operator*(sgl_Mat4 a, sgl_Mat4 b) {
    return sgl_Mat4_Mul_Mat4(a, b);
}

inline sgl_Vec4 operator*(sgl_Mat4 mat, sgl_Vec4 v) {
    return sgl_Mat4_Mul_Vec4(mat, v);
}

inline bool operator==(sgl_Mat4 a, sgl_Mat4 b) {
    return sgl_Mat4_Compare(a, b);
}

inline bool operator!=(sgl_Mat4 a, sgl_Mat4 b) {
    return !(a == b);
}

#endif