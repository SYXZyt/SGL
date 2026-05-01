#include "VertexArray.h"

void sgl::VertexArray::SetupLayout() const
{
    const auto& elements = mLayout.GetElements();

    glVertexArrayVertexBuffer(mVAO, 0, mVBO, 0, mLayout.Stride());

    for (const auto& e : elements)
    {
        glEnableVertexArrayAttrib(mVAO, e.index);

        if (e.integer)
        {
            glVertexArrayAttribIFormat(
                mVAO,
                e.index,
                e.count,
                e.type,
                e.offset
            );
        }
        else
        {
            glVertexArrayAttribFormat(
                mVAO,
                e.index,
                e.count,
                e.type,
                e.normalised,
                e.offset
            );
        }

        glVertexArrayAttribBinding(mVAO, e.index, 0);
    }
}

void sgl::VertexArray::Bind()
{
    if (mVertexData.empty())
        return;

    if (mVAO == 0)
        glCreateVertexArrays(1, &mVAO);

    if (mVBO == 0)
        glCreateBuffers(1, &mVBO);

    if (mIsIndexed && mEBO == 0)
        glCreateBuffers(1, &mEBO);

    if (mNeedsUpdate)
    {
        glNamedBufferData(
            mVBO,
            mVertexData.size(),
            mVertexData.data(),
            GL_DYNAMIC_DRAW
        );

        mNeedsUpdate = false;
    }

    if (mIsIndexed && !mIndexData.empty() && mIndexNeedsUpdate)
    {
        if (mIndexNeedsUpdate)
        {
            glNamedBufferData(
                mEBO,
                mIndexData.size() * sizeof(uint32),
                mIndexData.data(),
                GL_DYNAMIC_DRAW
            );

            mIndexNeedsUpdate = false;
        }
    }

    glVertexArrayVertexBuffer(mVAO, 0, mVBO, 0, mLayout.Stride());
}