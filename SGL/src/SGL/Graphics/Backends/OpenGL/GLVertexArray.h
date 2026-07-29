#pragma once
#include <SGL/SGL.h>
#include <SGL/Graphics/VertexArray.h>
#include <SGL/Graphics/VertexLayout.h>

SGL_BEGIN

/// @brief OpenGL vertex array
typedef struct sgl_GLVertexArray sgl_sealed
{
    sgl_VertexArray base;
    gluint vao, vbo, ebo;

} sgl_GLVertexArray;

/// @brief Create an OpenGL vertex array
SGL_API extern sgl_GLVertexArray* sgl_GLVertexArray_New(uint32 vertexSize, sgl_VertexLayout* layout);

SGL_END