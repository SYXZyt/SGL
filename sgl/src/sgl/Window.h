#pragma once
#include <sgl/sgl.h>
#include <SDL3/SDL.h>
#include <sgl/Maths/Vec2.h>
#include <sgl/String.h>

namespace sgl {
    class Game;
}

namespace sgl
{
    class Window final
    {
        friend class sgl::Game;

    public:
        struct Config final
        {
            SGL_API static const Config& Default;

            uint32 width = 1920;
            uint32 height = 1080;
            String title = u8"sgl";
            bool resizable = true;
        };

    private:
        Vec2 mScreenSize;
        Vec2 mHalfScreenSize;

        SDL_Window* mWindow;
        SDL_GLContext mGLContext = nullptr;

        bool mWantClose = false;

        void PollEvents();

    public:
        SDL_GLContext GLContext() const {
            return mGLContext;
        }

        SDL_Window* SDLWindow() const {
            return mWindow;
        }

        const Vec2& ScreenSize() const {
            return mScreenSize;
        }

        const Vec2& HalfScreenSize() const {
            return mHalfScreenSize;
        }
        
        bool WantClose() const {
            return mWantClose;
        }

        explicit Window(const Config& cfg);
        ~Window();
    };
}