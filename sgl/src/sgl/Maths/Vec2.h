#pragma once
#include <SGL/SGL.h>

SGL_BEGIN

#pragma region Vec2

typedef struct sgl_Vec2
{
    union
    {
        struct { float x, y; };
        struct { float u, v; };
        struct { float width, height; };
        float values[2];
    };
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
        struct { float x, y; };
        struct { float u, v; };
        struct { float width, height; };
        float values[2];
    };
} sgl_Vec2i;

SGL_API extern const sgl_Vec2i sgl_Vec2i_Zero;
SGL_API extern const sgl_Vec2i sgl_Vec2i_One;
SGL_API extern const sgl_Vec2i sgl_Vec2i_Left;
SGL_API extern const sgl_Vec2i sgl_Vec2i_Right;
SGL_API extern const sgl_Vec2i sgl_Vec2i_Up;
SGL_API extern const sgl_Vec2i sgl_Vec2i_Down;
SGL_API extern const sgl_Vec2i sgl_Vec2i_Inf;
SGL_API extern const sgl_Vec2i sgl_Vec2i_NegativeInf;

SGL_API extern sgl_Vec2i sgl_Vec2i_New_Scalar(float scalar);
SGL_API extern sgl_Vec2i sgl_Vec2i_New_ScalarXY(float x, float y);

SGL_API extern sgl_Vec2i sgl_Vec2i_Add_Vec2i(sgl_Vec2i a, sgl_Vec2i b);
SGL_API extern sgl_Vec2i sgl_Vec2i_Sub_Vec2i(sgl_Vec2i a, sgl_Vec2i b);
SGL_API extern sgl_Vec2i sgl_Vec2i_Mul_Vec2i(sgl_Vec2i a, sgl_Vec2i b);
SGL_API extern sgl_Vec2i sgl_Vec2i_Div_Vec2i(sgl_Vec2i a, sgl_Vec2i b);

SGL_API extern sgl_Vec2i sgl_Vec2i_Mul_Scalar(sgl_Vec2i v, float scalar);
SGL_API extern sgl_Vec2i sgl_Vec2i_Div_Scalar(sgl_Vec2i v, float scalar);

SGL_API extern sgl_Vec2i sgl_Vec2i_Negate(sgl_Vec2i v);

SGL_API extern bool sgl_Vec2i_Compare(sgl_Vec2i a, sgl_Vec2i b);

#pragma endregion

SGL_END