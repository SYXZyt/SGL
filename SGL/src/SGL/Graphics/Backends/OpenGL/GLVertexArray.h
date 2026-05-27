#pragma once
#include <SGL/SGL.h>
#include <SGL/Graphics/VertexArray.h>
#include <SGL/Graphics/VertexLayout.h>

SGL_BEGIN

typedef struct sgl_GLVertexArray sgl_sealed
{
    sgl_VertexArray base;
    gluint vao, vbo;

} sgl_GLVertexArray;

SGL_API extern sgl_GLVertexArray* sgl_GLVertexArray_New(uint32 vertexSize, sgl_VertexLayout* layout);

SGL_END