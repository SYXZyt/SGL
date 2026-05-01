#pragma once
#include <sgl/sgl.h>
#include <sgl/Collections/List.h>
#include <glad/glad.h>

namespace sgl
{
    enum class VertexElementType : uint8
    {
        FLOAT,
        FLOAT2,
        FLOAT3,
        FLOAT4,
        UINT,
        UINT2,
        UINT3,
        UINT4,
    };

    struct VertexElement final
    {
        gluint index = 0;
        glint count = 0;
        GLenum type = GL_FLOAT;
        bool normalised = false;
        uint32 offset = 0;
        bool integer = false;
    };

    class VertexLayout final
    {
    private:
        static void GetFormat(VertexElementType type, glint& count, GLenum& typeOut, bool& integer);
        static uint32 GetFormatSize(glint count,  GLenum type);

        List<VertexElement> mElements;
        uint32 mOffset = 0;
        gluint mNextIndex = 0;

    public:
        const List<VertexElement>& GetElements() const {
            return mElements;
        }

        uint32 Stride() const {
            return mOffset;
        }

        SGL_API void Add(VertexElementType type);
    };
}