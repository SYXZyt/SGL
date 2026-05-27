#include "GLVertexArray.h"
#include <glad/glad.h>
#include <SGL/Util/Memory.h>

#define GetSelf sgl_GLVertexArray* self = (sgl_GLVertexArray*)va;

static void GLDestroy(sgl_VertexArray* va)
{
    GetSelf;
     
    glDeleteBuffers(1, &self->vbo);
    glDeleteVertexArrays(1, &self->vao);

    sgl_VertexLayout_Destroy(va->layout);
    sgl::Memory::Delete(self);
}

static void GLBind(sgl_VertexArray* va)
{
    GetSelf;

    if (self->vao == 0 || va->vertexCount == 0)
        return;

    if (va->needsVertexUpload)
    {
        glNamedBufferData(self->vbo, (GLsizeiptr)(va->vertexCount * va->vertexSize), va->vertexData, GL_DYNAMIC_DRAW);
        va->needsVertexUpload = false;
    }

    glBindVertexArray(self->vao);
}

static void GLSetLayout(sgl_VertexArray* va)
{
    GetSelf;

    glBindVertexArray(self->vao);
    glVertexArrayVertexBuffer(self->vao, 0, self->vbo, 0, va->layout->stride);
    sgl_VertexLayout_Bind(va->layout);
    glBindVertexArray(0);
    sgl_VertexLayout_Bind(va->layout);
}

static sgl_VertexArrayVTable gGLVTable =
{
    .Destroy = &GLDestroy,
    .Bind = &GLBind,
    .CreateLayout = &GLSetLayout,
};

sgl_GLVertexArray* sgl_GLVertexArray_New(uint32 vertexSize)
{
    sgl_GLVertexArray* va = sgl::Memory::New<sgl_GLVertexArray>();

    va->base.vtable = &gGLVTable;

    va->base.vertexData = nullptr;
    va->base.vertexCount = 0;
    va->base.vertexCapacity = 0;
    va->base.vertexSize = vertexSize;

    va->base.needsVertexUpload = true;
    va->base.layoutDirty = true;

    glGenVertexArrays(1, &va->vao);
    glGenBuffers(1, &va->vbo);

    return va;
}