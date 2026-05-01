#include <sgl/sgl.h>
#include <sgl/Window.h>
#include <sgl/Game.h>
#include <sgl/Event.h>

class MyGame final : public sgl::Game
{
public:
    void Update() override
    {
        
    }

    void Render() override
    {

    }

    MyGame() : sgl::Game(sgl::Window::Config::Default) {
    }
};

static void OnEvent() {
    std::printf("On event function\n");
}

int main(int, char**)
{
    MyGame game;
    game.Run();

    return 0;
}