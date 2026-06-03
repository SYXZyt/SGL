#include "Mat4.h"
#include <cstring>

const sgl_Mat4 sgl_Mat4_Identity = sgl_Mat4_New_Diagonal(1.f);

sgl_Mat4 sgl_Mat4_New() {
    return sgl_Mat4_Identity;
}

sgl_Mat4 sgl_Mat4_New_Diagonal(float diagonal)
{
    sgl_Mat4 mat = sgl_Mat4_Identity;

    std::memset(mat.values, 0, sizeof(float) * 16);
    for (int i = 0; i < 4; ++i)
        mat.column[i][i] = diagonal;

    return mat;
}

sgl_Mat4 sgl_Mat4_New_Floats(const float* values)
{
    sgl_Mat4 mat = sgl_Mat4_Identity;

    std::memcpy(mat.values, values, sizeof(float) * 16);
    return mat;
}