#include <SGL/Window.h>
#include <SGL/Maths/Vec2.h>
#include <SGL/Util/ANSI.h>
#include <stdio.h>
#include <SGL/Util/Logger.h>

int main(int argc, char** argv)
{
    sgl_Logger_Init();
    sgl_Logger_Shutdown();

    return 0;
}