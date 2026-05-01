#pragma once
#include <sgl/sgl.h>
#include <sgl/Colour.h>
#include <sgl/Maths/Vec2.h>
#include <sgl/Ptr.h>
#include <SDL3/SDL.h>

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

    class GraphicsDevice final
    {
    private:
        Colour mClearColour;
        SDL_Window* mWindow;

        uint32 mWidth;
        uint32 mHeight;


    public:
        void SetClearColour(const Colour& colour);

        void Resize(const Vec2i& newsize);

        void Clear(ClearFlag flag);
        void Present();

        void Init();
        void Shutdown();
    };
}