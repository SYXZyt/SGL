#include "VertexLayout.h"

void sgl::VertexLayout::GetFormat(VertexElementType type, glint &count, GLenum &typeOut, bool &integer)
{
    switch (type)
    {
        case VertexElementType::FLOAT:
            count = 1;
            typeOut = GL_FLOAT;
            integer = false;
            return;

        case VertexElementType::FLOAT2:
            count = 2;
            typeOut = GL_FLOAT;
            integer = false;
            return;

        case VertexElementType::FLOAT3:
            count = 3;
            typeOut = GL_FLOAT;
            integer = false;
            return;

        case VertexElementType::FLOAT4:
            count = 4;
            typeOut = GL_FLOAT;
            integer = false;
            return;

        case VertexElementType::UINT:
            count = 1;
            typeOut = GL_UNSIGNED_INT;
            integer = true;
            return;

        case VertexElementType::UINT2:
            count = 2;
            typeOut = GL_UNSIGNED_INT;
            integer = true;
            return;

        case VertexElementType::UINT3:
            count = 3;
            typeOut = GL_UNSIGNED_INT;
            integer = true;
            return;

        case VertexElementType::UINT4:
            count = 4;
            typeOut = GL_UNSIGNED_INT;
            integer = true;
            return;
    }
}

uint32 sgl::VertexLayout::GetFormatSize(glint count, GLenum type)
{
    uint32 elementSize = 0;

    switch (type)
    {
        case GL_FLOAT:
        case GL_UNSIGNED_INT:
            elementSize = 4;
            break;
    }

    return count * elementSize;
}

void sgl::VertexLayout::Add(VertexElementType type)
{
    glint count;
    GLenum glType;
    bool isInteger;

    GetFormat(type, count, glType, isInteger);

    VertexElement element;
    element.index = mNextIndex++;
    element.count = count;
    element.type = glType;
    element.normalised = false;
    element.offset = mOffset;
    element.integer = isInteger;

    mElements.push_back(element);

    mOffset += GetFormatSize(count, glType);
}

void sgl::VertexLayout::Apply() const
{
    for (const VertexElement& e : mElements)
    {
        glEnableVertexAttribArray(e.index);

        if (e.integer)
        {
            glVertexAttribIPointer(
                e.index,
                e.count,
                e.type,
                Stride(),
                (const void*)(uintptr_t)e.offset
            );
        }
        else
        {
            glVertexAttribPointer(
                e.index,
                e.count,
                e.type,
                e.normalised,
                Stride(),
                (const void*)(uintptr_t)e.offset
            );
        }
    }
}