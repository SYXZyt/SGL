#pragma once
#include <SGL/Graphics/Texture2D.h>

SGL_BEGIN

struct sgl_Texture2DArray;

typedef struct sgl_Texture2DArray sgl_sealed
{
    sgl_Texture2D base;
} sgl_Texture2DArray;

SGL_END