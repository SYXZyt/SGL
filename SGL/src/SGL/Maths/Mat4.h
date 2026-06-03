#pragma once
#include <SGL/SGL.h>

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

SGL_END