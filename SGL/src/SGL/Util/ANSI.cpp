#include "ANSI.h"
#include <string>

sgl_String sgl_ANSI_MakeRGB(uint8 r, uint8 g, uint8 b)
{
    std::string rs = std::to_string((int)r);
    std::string gs = std::to_string((int)g);
    std::string bs = std::to_string((int)b);

    std::string ansi;
    ansi += "\033[38;2;" + rs;
    ansi += ';' + gs;
    ansi += ';' + bs;
    ansi += 'm';

    sgl_String sglStr = sgl_MakeStringCpp(ansi);
    return sglStr;
}