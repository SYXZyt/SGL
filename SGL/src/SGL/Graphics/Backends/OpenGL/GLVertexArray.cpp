#include "GLVertexArray.h"
#include <glad/glad.h>
#include <SGL/Util/Memory.h>
#include <SGL/Graphics/Backends/OpenGL/OpenGLThreadSync.h>

#define GetSelf sgl_GLVertexArray* self = (sgl_GLVertexArray*)va

static void GLDestroy(sgl_VertexArray* va)
{
    GetSelf;

    sgl_OpenGLThreadSync_DeleteVBO(self->vbo);
    sgl_OpenGLThreadSync_DeleteEBO(self->ebo);
    sgl_OpenGLThreadSync_DeleteVAO(self->vao);

    if (self->instanceVbo)
        sgl_OpenGLThreadSync_DeleteVBO(self->instanceVbo);

    sgl_VertexLayout_Destroy(va->layout);
    sgl::Memory::Delete(self);
}

static void ApplyVertexLayout(GLuint vbo, GLuint instanceVbo, GLuint vao, sgl_VertexLayout* layout, uint32 vertexSize, uint32 instanceSize);

static void GLEnsureGPUResources(sgl_GLVertexArray* self, sgl_VertexArray* va)
{
    if (self->gpuLoaded)
        return;

    glCreateVertexArrays(1, &self->vao);
    glCreateBuffers(1, &self->vbo);
    glCreateBuffers(1, &self->ebo);

    if (va->instanceSize > 0)
        glCreateBuffers(1, &self->instanceVbo);

    ApplyVertexLayout(self->vbo, self->instanceVbo, self->vao, va->layout, va->vertexSize, va->instanceSize);
    glVertexArrayElementBuffer(self->vao, self->ebo);

    self->gpuLoaded = true;
}

static void GLBind(sgl_VertexArray* va)
{
    GetSelf;

    if (!self->gpuLoaded)
        GLEnsureGPUResources(self, va);

    glBindVertexArray(self->vao);

    if (va->needsVertexUpload && va->vertexData != nullptr && va->vertexCount > 0)
    {
        glNamedBufferData(self->vbo, (GLsizeiptr)(va->vertexCount * va->vertexSize), va->vertexData, GL_DYNAMIC_DRAW);
        va->needsVertexUpload = false;
    }

    if (va->needsIndexUpload && va->indexData != nullptr && va->indexCount > 0)
    {
        glNamedBufferData(self->ebo, (GLsizeiptr)(va->indexCount * sizeof(uint32)), va->indexData, GL_DYNAMIC_DRAW);
        va->needsIndexUpload = false;
    }

    if (va->needsInstanceUpload && va->instanceData != nullptr && va->instanceCount > 0)
    {
        glNamedBufferData(self->instanceVbo, (GLsizeiptr)(va->instanceCount * va->instanceSize), va->instanceData, GL_STREAM_DRAW);
        va->needsInstanceUpload = false;
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

static void ApplyVertexLayout(GLuint vbo, GLuint instanceVbo, GLuint vao, sgl_VertexLayout* layout, uint32 vertexSize, uint32 instanceSize)
{
    glBindVertexArray(vao);

    glVertexArrayVertexBuffer(vao, 0, vbo, 0, vertexSize);

    if (instanceSize > 0)
    {
        glVertexArrayVertexBuffer(vao, 1, instanceVbo, 0, instanceSize);
        glVertexArrayBindingDivisor(vao, 1, 1);
    }

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

        glVertexAttribBinding(attribIndex, e->perInstance ? 1 : 0);
    }

    glBindVertexArray(0);
}

sgl_GLVertexArray* sgl_GLVertexArray_New(uint32 vertexSize, sgl_VertexLayout* layout, uint32 instanceSize)
{
    sgl_GLVertexArray* va = sgl::Memory::New<sgl_GLVertexArray>();

    va->base.vtable = &gGLVTable;

    va->base.vertexData = nullptr;
    va->base.vertexCount = 0;
    va->base.vertexCapacity = 0;
    va->base.vertexSize = vertexSize;

    va->base.indexData = nullptr;
    va->base.indexCount = 0;
    va->base.indexCapacity = 0;

    va->base.instanceData = nullptr;
    va->base.instanceCount = 0;
    va->base.instanceCapacity = 0;
    va->base.instanceSize = instanceSize;

    va->base.needsVertexUpload = true;
    va->base.needsIndexUpload = true;
    va->base.needsInstanceUpload = true;
    va->base.layoutDirty = true;

    va->vao = 0;
    va->vbo = 0;
    va->ebo = 0;
    va->instanceVbo = 0;
    va->gpuLoaded = false;

    return va;
}