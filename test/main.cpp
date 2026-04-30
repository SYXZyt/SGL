#include <sgl/sgl.h>
#include <sgl/Window.h>
#include <sgl/Game.h>

class MyGame final : public sgl::Game
{
public:
    void Update() override
    {

    }

    void Render() override
    {

    }

    MyGame() : sgl::Game(sgl::Window::Config::Default) {}
};

int main(int, char**)
{
    MyGame game;
    game.Run();

    return 1;
}