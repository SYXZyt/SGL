#pragma once
#include <sgl/sgl.h>
#include <sgl/Colour.h>

namespace sgl
{
    enum class ClearFlag : uint8
    {
        NONE    SGL_ENUM_FLAG(0),
        COLOUR  SGL_ENUM_FLAG(1),
        DEPTH   SGL_ENUM_FLAG(2),
    };

    SGL_API extern ClearFlag operator|(ClearFlag a, ClearFlag b);
    SGL_API extern ClearFlag operator&(ClearFlag a, ClearFlag b);

    SGL_API extern ClearFlag& operator|=(ClearFlag& a, ClearFlag b);
    SGL_API extern ClearFlag& operator&=(ClearFlag& a, ClearFlag b);

    class GraphicsDevice
    {
    protected:
        Colour mClearColour;

        uint32 mWidth;
        uint32 mHeight;

    public:
        virtual ~GraphicsDevice() = default;
    };
}