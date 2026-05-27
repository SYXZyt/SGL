#pragma once
#include <SGL/Graphics/VertexLayout.h>

SGL_BEGIN

typedef struct sgl_GLVertexLayout sgl_sealed
{
    sgl_VertexLayout base;
} sgl_GLVertexLayout;

SGL_API extern sgl_GLVertexLayout* sgl_GLVertexLayout_Create();

SGL_END