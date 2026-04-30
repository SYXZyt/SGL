#include "Game.h"

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
    mWindow(Window(cfg)) {
}