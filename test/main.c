#include <SGL/Window.h>
#include <SGL/Maths/Vec2.h>
#include <SGL/Util/ANSI.h>
#include <stdio.h>
#include <SGL/Util/Logger.h>
#include <SGL/Graphics/GraphicsDevice.h>

int main(int argc, char** argv)
{
    sgl_Logger_Init();

    sgl_EngineConfig cfg = sgl_EngineConfig_Default;
    sgl_Window* window = sgl_Window_Create(cfg);
    sgl_GraphicsDevice* gpu = sgl_GraphicsDevice_Create(window);

    while (!window->wantsClose)
    {
        sgl_Window_PollEvents(window);

        sgl_GraphicsDevice_Clear(gpu, sgl_ClearFlag_COLOUR | sgl_ClearFlag_DEPTH);
        sgl_GraphicsDevice_Present(gpu);
    }

    sgl_Window_Destroy(window);
    sgl_Logger_Shutdown();

    return 0;
}