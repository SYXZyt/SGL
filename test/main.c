#include <SGL/Window.h>
#include <SGL/Maths/Vec2.h>
#include <SGL/Util/ANSI.h>
#include <stdio.h>
#include <SGL/Util/Logger.h>

static void OnResize(sgl_Window* window, sgl_Vec2i newSize, void* userdata) {
    printf("Resized: %dx%d\n", newSize.width, newSize.height);
}

int main(int argc, char** argv)
{
    sgl_Logger_Init();

    sgl_EngineConfig cfg = sgl_EngineConfig_Default;
    sgl_Window* window = sgl_Window_Create(cfg);

    sgl_Window_RegisterResize(window, &OnResize, NULL);

    while (!window->wantsClose)
    {
        sgl_Window_PollEvents(window);
    }

    sgl_Window_DeregisterResize(window, &OnResize);

    sgl_Window_Destroy(window);
    sgl_Logger_Shutdown();

    return 0;
}