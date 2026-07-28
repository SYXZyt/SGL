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

sgl_Mat4 sgl_Mat4_Mul_Mat4(sgl_Mat4 a, sgl_Mat4 b)
{
    sgl_Mat4 result{};

    for (int col = 0; col < 4; ++col)
    {
        for (int row = 0; row < 4; ++row)
        {
            float sum = 0.f;
            for (int k = 0; k < 4; ++k)
                sum += a.column[k][row] * b.column[col][k];

            result.column[col][row] = sum;
        }
    }

    return result;
}

sgl_Vec4 sgl_Mat4_Mul_Vec4(sgl_Mat4 mat, sgl_Vec4 v)
{
    sgl_Vec4 result{};

    for (int row = 0; row < 4; ++row)
    {
        float sum = 0.f;
        for (int k = 0; k < 4; ++k)
            sum += mat.column[k][row] * v.values[k];

        result.values[row] = sum;
    }

    return result;
}

bool sgl_Mat4_Compare(sgl_Mat4 a, sgl_Mat4 b)
{
    for (int i = 0; i < 16; ++i)
    {
        if (a.values[i] != b.values[i])
            return false;
    }

    return true;
}