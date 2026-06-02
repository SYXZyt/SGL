#include "GLVertexArray.h"
#include <glad/glad.h>
#include <SGL/Util/Memory.h>

#define GetSelf sgl_GLVertexArray* self = (sgl_GLVertexArray*)va

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

    if (self->vbo == 0 || self->vao == 0)
        return;

    glBindVertexArray(self->vao);

    if (va->needsVertexUpload && va->vertexData != nullptr && va->vertexCount > 0)
    {
        glNamedBufferData(self->vbo, (GLsizeiptr)(va->vertexCount * va->vertexSize), va->vertexData, GL_DYNAMIC_DRAW);
        va->needsVertexUpload = false;
    }
}

static sgl_VertexArrayVTable gGLVTable =
{
    .Destroy = &GLDestroy,
    .Bind = &GLBind,
};

static void GetGLType(sgl_VertexElementType type, GLenum* outType, GLint* outComponentCount)
{
    switch (type)
    {
        case sgl_VertexElementType_FLOAT:
            *outType = GL_FLOAT;
            *outComponentCount = 1;
            return;

        case sgl_VertexElementType_INT8:
            *outType = GL_BYTE;
            *outComponentCount = 1;
            return;

        case sgl_VertexElementType_INT16:
            *outType = GL_SHORT;
            *outComponentCount = 1;
            return;

        case sgl_VertexElementType_INT32:
            *outType = GL_INT;
            *outComponentCount = 1;
            return;

        case sgl_VertexElementType_UINT8:
            *outType = GL_UNSIGNED_BYTE;
            *outComponentCount = 1;
            return;

        case sgl_VertexElementType_UINT16:
            *outType = GL_UNSIGNED_SHORT;
            *outComponentCount = 1;
            return;

        case sgl_VertexElementType_UINT32:
            *outType = GL_UNSIGNED_INT;
            *outComponentCount = 1;
            return;

        case sgl_VertexElementType_VEC2:
            *outType = GL_FLOAT;
            *outComponentCount = 2;
            return;

        case sgl_VertexElementType_VEC3:
            *outType = GL_FLOAT;
            *outComponentCount = 3;
            return;

        case sgl_VertexElementType_VEC4:
            *outType = GL_FLOAT;
            *outComponentCount = 4;
            return;
    }

    *outType = GL_FLOAT;
    *outComponentCount = 1;
}

static void ApplyVertexLayout(GLuint vbo, GLuint vao, sgl_VertexLayout* layout, uint32 vertexSize)
{
    glBindVertexArray(vao);

    glVertexArrayVertexBuffer(vao, 0, vbo, 0, vertexSize);

    for (uint32 i = 0; i < layout->elementCount; i++)
    {
        sgl_VertexElement* e = &layout->elements[i];

        GLenum glType;
        GLint componentCount;
        GetGLType(e->type, &glType, &componentCount);

        GLuint attribIndex = i;

        glEnableVertexAttribArray(attribIndex);

        glVertexAttribFormat(
            attribIndex,
            componentCount,
            glType,
            GL_FALSE,
            (GLuint)e->offset
        );

        glVertexAttribBinding(attribIndex, 0);
    }

    glBindVertexArray(0);
}

sgl_GLVertexArray* sgl_GLVertexArray_New(uint32 vertexSize, sgl_VertexLayout* layout)
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

    ApplyVertexLayout(va->vbo, va->vao, layout, vertexSize);

    return va;
}