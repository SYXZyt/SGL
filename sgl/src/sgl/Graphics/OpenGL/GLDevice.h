#pragma once
#include <sgl/GraphicsDevice.h>
#include <SDL3/SDL.h>
#include <sgl/Memory/Memory.h>

namespace sgl::Graphics
{
    class GLDevice final : public GraphicsDevice
    {
        SGL_ALLOCATOR_FRIEND;

    private:
        uint32 mWidth = 0;
        uint32 mHeight = 0;
        SDL_Window* mWindow;

    public:
        void Resize(const Vec2i& newsize) override;

        void SetClearColour(const Colour& colour) override;

        void Clear(ClearFlag flag) override;
        void Present() override;

        void Init() override;
        void Shutdown() override;
    };
}