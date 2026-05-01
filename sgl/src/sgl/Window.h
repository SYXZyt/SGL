#pragma once
#include <sgl/sgl.h>
#include <SDL3/SDL.h>
#include <sgl/Maths/Vec2.h>
#include <sgl/String.h>
#include <sgl/GraphicsAPI.h>
#include <sgl/Event.h>

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

            String title = u8"sgl";
            uint32 width = 1920;
            uint32 height = 1080;
            GraphicsAPI graphicsApi = GraphicsAPI::OPENGL;
            bool resizable = true;
        };

    private:
        Event<const Vec2i&> mResize;

        Vec2i mScreenSize;
        Vec2i mHalfScreenSize;

        SDL_Window* mWindow;
        SDL_GLContext mGLContext = nullptr;

#ifdef _WIN32
        HWND mHwnd;
#endif

        bool mWantClose = false;

        void PollEvents();

    public:
#ifdef _WIN32
        HWND GetWin32Handle() const {
            return mHwnd;
        }
#endif

        SDL_GLContext GLContext() const {
            return mGLContext;
        }

        SDL_Window* SDLWindow() const {
            return mWindow;
        }

        const Vec2i& ScreenSize() const {
            return mScreenSize;
        }

        const Vec2i& HalfScreenSize() const {
            return mHalfScreenSize;
        }
        
        bool WantClose() const {
            return mWantClose;
        }

        Event<const Vec2i&>& OnResize() {
            return mResize;
        }

        explicit Window(const Config& cfg);
        ~Window();
    };
}