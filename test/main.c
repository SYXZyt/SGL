#include <SGL/Window.h>
#include <SGL/Maths/Vec2.h>
#include <SGL/Util/ANSI.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    sgl_String ansi = sgl_ANSI_MakeRGB(155, 244, 45);

    printf("%sHello, World%s\n", ansi.str, sgl_ANSI_Reset);

    sgl_FreeString(ansi);

    return 0;
}