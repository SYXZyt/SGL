#pragma once
#include <sgl/sgl.h>
#include <sgl/Window.h>
#include <sgl/Ptr.h>
#include <sgl/GraphicsDevice.h>

namespace sgl
{
    class Game
    {
    private:
        static Game* sInstance;

        Window mWindow;
        Ptr<GraphicsDevice> mGraphics;

        void OnResize(const Vec2i& newsize);

    protected:
        Window& GetWindow() {
            return mWindow;
        }

        virtual void Initialise() {}
        virtual void Deinitialise() {}

        virtual void LoadContent() {}
        virtual void UnloadContent() {}

        virtual void Update() = 0;
        virtual void Render() = 0;

    public:
        GraphicsDevice& GetGraphics() {
            return mGraphics.Ref();
        }

        void Run();

        Game(const Window::Config& cfg);
        virtual ~Game() = default;
    };
}