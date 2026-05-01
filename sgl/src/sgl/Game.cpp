#include "Game.h"
#include <sgl/Util/Logger.h>

sgl::Game* sgl::Game::sInstance = nullptr;

void sgl::Game::OnResize(const Vec2i& newsize)
{
    mGraphics.Resize(newsize);
}

void sgl::Game::Run()
{
    LoadContent();

    mGraphics.SetClearColour(Colour::CornflowerBlue);

    while (!mWindow.WantClose())
    {
        mWindow.PollEvents();
    
        mGraphics.Clear(ClearFlag::COLOUR | ClearFlag::DEPTH);

        Update();
        Render();

        mGraphics.Present();
    }

    UnloadContent();
}

sgl::Game::Game(const Window::Config& cfg) :
    mWindow(Window(cfg)),
    mGraphics({})
{
    sInstance = this;

    mWindow.OnResize += std::pair(this, &Game::OnResize);

    mGraphics.Init();
}

sgl::Game::~Game()
{
    mWindow.OnResize -= std::pair(this, &Game::OnResize);

    mGraphics.Shutdown();
}