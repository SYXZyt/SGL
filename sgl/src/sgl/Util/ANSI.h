#pragma once
#include <sgl/sgl.h>
#include <sgl/String.h>
#include <sgl/Colour.h>

namespace sgl::ANSI
{
    static inline const String Reset = u8"\033[0m";

    namespace Colours
    {
        SGL_API extern String MakeRGB(uint8 r, uint8 g, uint8 b);

        static inline const String Black = u8"\033[30m";
        static inline const String Red = u8"\033[31m";
        static inline const String Green = u8"\033[32m";
        static inline const String Yellow = u8"\033[33m";
        static inline const String Blue = u8"\033[34m";
        static inline const String Magenta = u8"\033[35m";
        static inline const String Cyan = u8"\033[36m";
        static inline const String White = u8"\033[37m";

        namespace Logger
        {
            static inline const String Error = MakeRGB(247, 121, 121);
            static inline const String Warning = MakeRGB(247, 214, 114);
            static inline const String Info = MakeRGB(159, 146, 240);
            static inline const String Success = MakeRGB(145, 255, 128);
        }
    }

    namespace Effects
    {
        static inline const String Bold = u8"\033[1m";
        static inline const String Underline = u8"\033[4m";
        static inline const String Italic = u8"\033[3m";
        static inline const String Dim = u8"\033[2m";
        static inline const String Blink = u8"\033[5m";
        static inline const String Reverse = u8"\033[7m";
        static inline const String Hidden = u8"\033[8m";
        static inline const String Strike = u8"\033[9m";
    }

	SGL_API extern Colour ColourFromANSI(const String& ansiCode);
}