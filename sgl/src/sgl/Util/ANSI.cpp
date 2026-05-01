#include "ANSI.h"

sgl::String sgl::ANSI::Colours::MakeRGB(uint8 r, uint8 g, uint8 b) {
    return u8"\033[38;2;" + ToString((uint16)r) + u8";" + ToString((uint16)g) + u8";" + ToString((uint16)b) + u8"m";
}

sgl::Colour sgl::ANSI::ColourFromANSI(const String& ansiCode)
{
    std::string str = ConvertString<String, std::string>(ansiCode);
    const char* cstr = str.c_str();

    int r, g, b;
    r = g = b = 0;

    if (ansiCode.rfind(u8"\033[38;2;", 0) != 0)
        return Colour::Black;

    cstr += 7;

    if (sscanf(cstr, "%d;%d;%dm", &r, &g, &b) != 3)
        return Colour::Black;

    return Colour(r / 255.f, g / 255.f, b / 255.f);
}