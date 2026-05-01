#include "Game.h"

sgl::Game* sgl::Game::sInstance = nullptr;

void sgl::Game::OnResize(const Vec2i& newsize)
{

}

void sgl::Game::Run()
{
    LoadContent();

    while (!mWindow.WantClose())
    {
        mWindow.PollEvents();
    
        Update();
        Render();
    }

    UnloadContent();
}

sgl::Game::Game(const Window::Config& cfg) :
    mWindow(Window(cfg))
{
    mWindow.OnResize() += std::pair(this, &Game::OnResize);
    sInstance = this;
}