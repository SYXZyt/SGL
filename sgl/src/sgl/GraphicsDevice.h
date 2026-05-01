#pragma once
#include <sgl/sgl.h>
#include <sgl/Colour.h>
#include <sgl/Maths/Vec2.h>
#include <sgl/Ptr.h>
#include <sgl/GraphicsAPI.h>

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
        friend class Game;
    private:
        static Ptr<GraphicsDevice> Create(GraphicsAPI api);

    protected:
        Colour mClearColour;

        uint32 mWidth;
        uint32 mHeight;

    public:
        virtual void SetClearColour(const Colour& colour) {
            mClearColour = colour;
        }

        virtual void Resize(const Vec2i& newsize) = 0;

        virtual void Clear(ClearFlag flag) = 0;
        virtual void Present() = 0;

        virtual void Init() = 0;
        virtual void Shutdown() = 0;

        virtual ~GraphicsDevice() = default;
    };
}