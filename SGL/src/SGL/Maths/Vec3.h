 #pragma once
#include <SGL/SGL.h>

SGL_BEGIN

#pragma region Vec3

typedef struct sgl_Vec3 sgl_sealed
{
    union
    {
        struct { float x, y, z; };
        struct { float u, v, w; };
        struct { float r, g, b; };
        struct { float width, height, depth; };
        float values[3];
    };

#ifdef __cplusplus
    SGL_API static sgl_Vec3 Zero;
    SGL_API static sgl_Vec3 One;
    SGL_API static sgl_Vec3 Left;
    SGL_API static sgl_Vec3 Right;
    SGL_API static sgl_Vec3 Up;
    SGL_API static sgl_Vec3 Down;
    SGL_API static sgl_Vec3 Forward;
    SGL_API static sgl_Vec3 Backward;
    SGL_API static sgl_Vec3 Inf;
    SGL_API static sgl_Vec3 NegativeInf;
#endif
} sgl_Vec3;

SGL_API extern const sgl_Vec3 sgl_Vec3_Zero;
SGL_API extern const sgl_Vec3 sgl_Vec3_One;
SGL_API extern const sgl_Vec3 sgl_Vec3_Left;
SGL_API extern const sgl_Vec3 sgl_Vec3_Right;
SGL_API extern const sgl_Vec3 sgl_Vec3_Up;
SGL_API extern const sgl_Vec3 sgl_Vec3_Down;
SGL_API extern const sgl_Vec3 sgl_Vec3_Forward;
SGL_API extern const sgl_Vec3 sgl_Vec3_Backward;
SGL_API extern const sgl_Vec3 sgl_Vec3_Inf;
SGL_API extern const sgl_Vec3 sgl_Vec3_NegativeInf;

SGL_API extern sgl_Vec3 sgl_Vec3_New_Scalar(float scalar);
SGL_API extern sgl_Vec3 sgl_Vec3_New_ScalarXYZ(float x, float y, float z);

SGL_API extern sgl_Vec3 sgl_Vec3_Add_Vec3(sgl_Vec3 a, sgl_Vec3 b);
SGL_API extern sgl_Vec3 sgl_Vec3_Sub_Vec3(sgl_Vec3 a, sgl_Vec3 b);
SGL_API extern sgl_Vec3 sgl_Vec3_Mul_Vec3(sgl_Vec3 a, sgl_Vec3 b);
SGL_API extern sgl_Vec3 sgl_Vec3_Div_Vec3(sgl_Vec3 a, sgl_Vec3 b);

SGL_API extern sgl_Vec3 sgl_Vec3_Mul_Scalar(sgl_Vec3 v, float scalar);
SGL_API extern sgl_Vec3 sgl_Vec3_Div_Scalar(sgl_Vec3 v, float scalar);

SGL_API extern sgl_Vec3 sgl_Vec3_Negate(sgl_Vec3 v);

SGL_API extern bool sgl_Vec3_Compare(sgl_Vec3 a, sgl_Vec3 b);

#pragma endregion

#pragma region Vec3i

typedef struct sgl_Vec3i sgl_sealed
{
    union
    {
        struct { int x, y, z; };
        struct { int u, v, w; };
        struct { int r, g, b; };
        struct { int width, height, depth; };
        int values[3];
    };

#ifdef __cplusplus
    SGL_API static sgl_Vec3i Zero;
    SGL_API static sgl_Vec3i One;
    SGL_API static sgl_Vec3i Left;
    SGL_API static sgl_Vec3i Right;
    SGL_API static sgl_Vec3i Up;
    SGL_API static sgl_Vec3i Down;
    SGL_API static sgl_Vec3i Forward;
    SGL_API static sgl_Vec3i Backward;
    SGL_API static sgl_Vec3i Inf;
    SGL_API static sgl_Vec3i NegativeInf;
#endif
} sgl_Vec3i;

SGL_API extern const sgl_Vec3i sgl_Vec3i_Zero;
SGL_API extern const sgl_Vec3i sgl_Vec3i_One;
SGL_API extern const sgl_Vec3i sgl_Vec3i_Left;
SGL_API extern const sgl_Vec3i sgl_Vec3i_Right;
SGL_API extern const sgl_Vec3i sgl_Vec3i_Up;
SGL_API extern const sgl_Vec3i sgl_Vec3i_Down;
SGL_API extern const sgl_Vec3i sgl_Vec3i_Forward;
SGL_API extern const sgl_Vec3i sgl_Vec3i_Backward;
SGL_API extern const sgl_Vec3i sgl_Vec3i_Inf;
SGL_API extern const sgl_Vec3i sgl_Vec3i_NegativeInf;

SGL_API extern sgl_Vec3i sgl_Vec3i_New_Scalar(int scalar);
SGL_API extern sgl_Vec3i sgl_Vec3i_New_ScalarXYZ(int x, int y, int z);

SGL_API extern sgl_Vec3i sgl_Vec3i_Add_Vec3i(sgl_Vec3i a, sgl_Vec3i b);
SGL_API extern sgl_Vec3i sgl_Vec3i_Sub_Vec3i(sgl_Vec3i a, sgl_Vec3i b);
SGL_API extern sgl_Vec3i sgl_Vec3i_Mul_Vec3i(sgl_Vec3i a, sgl_Vec3i b);
SGL_API extern sgl_Vec3i sgl_Vec3i_Div_Vec3i(sgl_Vec3i a, sgl_Vec3i b);

SGL_API extern sgl_Vec3i sgl_Vec3i_Mul_Scalar(sgl_Vec3i v, int scalar);
SGL_API extern sgl_Vec3i sgl_Vec3i_Div_Scalar(sgl_Vec3i v, int scalar);

SGL_API extern sgl_Vec3i sgl_Vec3i_Negate(sgl_Vec3i v);

SGL_API extern bool sgl_Vec3i_Compare(sgl_Vec3i a, sgl_Vec3i b);

#pragma endregion

SGL_END

// If using C++, it is nice to have some operators
#ifdef __cplusplus

inline sgl_Vec3 operator+(sgl_Vec3 a, sgl_Vec3 b) {
    return sgl_Vec3_Add_Vec3(a, b);
}

inline sgl_Vec3 operator-(sgl_Vec3 a, sgl_Vec3 b) {
    return sgl_Vec3_Sub_Vec3(a, b);
}

inline sgl_Vec3 operator*(sgl_Vec3 a, sgl_Vec3 b) {
    return sgl_Vec3_Mul_Vec3(a, b);
}

inline sgl_Vec3 operator/(sgl_Vec3 a, sgl_Vec3 b) {
    return sgl_Vec3_Div_Vec3(a, b);
}

inline sgl_Vec3 operator*(sgl_Vec3 a, float b) {
    return sgl_Vec3_Mul_Scalar(a, b);
}

inline sgl_Vec3 operator/(sgl_Vec3 a, float b) {
    return sgl_Vec3_Div_Scalar(a, b);
}

inline sgl_Vec3 operator-(sgl_Vec3 v) {
    return sgl_Vec3_Negate(v);
}

inline bool operator==(sgl_Vec3 a, sgl_Vec3 b) {
    return sgl_Vec3_Compare(a, b);
}

inline bool operator!=(sgl_Vec3 a, sgl_Vec3 b) {
    return !(a == b);
}


inline sgl_Vec3i operator+(sgl_Vec3i a, sgl_Vec3i b) {
    return sgl_Vec3i_Add_Vec3i(a, b);
}

inline sgl_Vec3i operator-(sgl_Vec3i a, sgl_Vec3i b) {
    return sgl_Vec3i_Sub_Vec3i(a, b);
}

inline sgl_Vec3i operator*(sgl_Vec3i a, sgl_Vec3i b) {
    return sgl_Vec3i_Mul_Vec3i(a, b);
}

inline sgl_Vec3i operator/(sgl_Vec3i a, sgl_Vec3i b) {
    return sgl_Vec3i_Div_Vec3i(a, b);
}

inline sgl_Vec3i operator*(sgl_Vec3i a, int b) {
    return sgl_Vec3i_Mul_Scalar(a, b);
}

inline sgl_Vec3i operator/(sgl_Vec3i a, int b) {
    return sgl_Vec3i_Div_Scalar(a, b);
}

inline sgl_Vec3i operator-(sgl_Vec3i v) {
    return sgl_Vec3i_Negate(v);
}

inline bool operator==(sgl_Vec3i a, sgl_Vec3i b) {
    return sgl_Vec3i_Compare(a, b);
}

inline bool operator!=(sgl_Vec3i a, sgl_Vec3i b) {
    return !(a == b);
}

#endif