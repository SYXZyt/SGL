#include "GLVertexLayout.h"
#include <SGL/Util/Memory.h>
#include <glad/glad.h>

#define GetSelf sgl_GLVertexLayout* self = (sgl_GLVertexLayout*)layout;

static void EnsureCapacity(sgl_VertexLayout* layout)
{
    if (layout->elementCount < layout->elementCapacity)
        return;

    uint32 newCap = layout->elementCapacity == 0 ? 4 : layout->elementCapacity * 2;

    layout->elements = (sgl_VertexElement*)sgl_Realloc(layout->elements, newCap * sizeof(sgl_VertexElement));
    layout->elementCapacity = newCap;
}

static void GLAdd(sgl_VertexLayout* layout, sgl_VertexElementType type)
{
    EnsureCapacity(layout);

    uint32 count = sgl_VertexLayout_GetComponentCount(type);

    sgl_VertexElement* e = &layout->elements[layout->elementCount++];

    e->index = layout->nextIndex++;
    e->count = count;
    e->offset = layout->stride;
    e->integer = (type >= sgl_VertexElementType_UINT);

    layout->stride += count * 4;
}

static void GLDestroy(sgl_VertexLayout* layout)
{
    GetSelf;

    sgl_Free(layout->elements);
    sgl::Memory::Delete(self);
}

static void GLBind(sgl_VertexLayout* layout)
{
    GetSelf;

    uint32 stride = layout->stride;

    for (uint32 i = 0; i < layout->elementCount; ++i)
    {
        const sgl_VertexElement* e = &layout->elements[i];

        GLenum type = GL_FLOAT;
        if (e->integer)
            type = GL_UNSIGNED_INT;

        glEnableVertexAttribArray(e->index);
        if (e->integer)
        {
            glVertexAttribIPointer(
                e->index,
                (GLint)e->count,
                type,
                (GLsizei)stride,
                (const void*)(uintptr_t)e->offset
            );
        }
        else
        {
            glVertexAttribPointer(
                e->index,
                (GLint)e->count,
                type,
                GL_FALSE,
                (GLsizei)stride,
                (const void*)(uintptr_t)e->offset
            );
        }
    }
}

static const sgl_VertexLayoutVTable gGLVTable =
{
    .Destroy = &GLDestroy,
    .Add = &GLAdd,
    .Bind = &GLBind,
};

sgl_GLVertexLayout* sgl_GLVertexLayout_Create()
{
    sgl_GLVertexLayout* layout = sgl::Memory::New<sgl_GLVertexLayout>();
    
    layout->base.vtable = &gGLVTable;
    layout->base.elements = nullptr;
    layout->base.elementCount = layout->base.elementCapacity = 0;
    layout->base.stride = 0;
    layout->base.nextIndex = 0;

    return layout;
}