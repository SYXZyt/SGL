#pragma once
#include <sgl/sgl.h>
#include <sgl/Window.h>

namespace sgl
{
    class Game
    {
    private:
        Window mWindow;

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
        void Run();

        Game(const Window::Config& cfg);
    };
}