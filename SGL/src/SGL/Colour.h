#pragma once
#include <SGL/SGL.h>
#include <SGL/Maths/Vec3.h>
#include <SGL/Maths/Vec4.h>

#define SGL_DEF_COLOUR(name) SGL_API extern const sgl_Colour sgl_Col_##name

SGL_BEGIN

typedef uint32 sgl_PackedColour;

typedef struct sgl_Colour sgl_sealed
{
    float r, g, b, a;
} sgl_Colour;

SGL_API extern sgl_Vec3 sgl_Colour_ToVec3(sgl_Colour c);
SGL_API extern sgl_Vec4 sgl_Colour_ToVec4(sgl_Colour c);

SGL_API extern sgl_PackedColour sgl_Colour_Pack(sgl_Colour colour);
SGL_API extern sgl_Colour sgl_Colour_Unpack(sgl_PackedColour colour);

SGL_API extern sgl_Colour sgl_Colour_Float(float r, float g, float b); // Assumes a = 1
SGL_API extern sgl_Colour sgl_Colour_FloatA(float r, float g, float b, float a);
SGL_API extern sgl_Colour sgl_Colour_Byte(uint8 r, uint8 g, uint8 b); // Assumes a = 255
SGL_API extern sgl_Colour sgl_Colour_ByteA(uint8 r, uint8 g, uint8 b, uint8 a);

// Colours taken from https://www.w3.org/TR/css-color-4/#named-colors
SGL_DEF_COLOUR(AliceBlue);
SGL_DEF_COLOUR(AntiqueWhite);
SGL_DEF_COLOUR(Aqua);
SGL_DEF_COLOUR(Aquamarine);
SGL_DEF_COLOUR(Azure);
SGL_DEF_COLOUR(Beige);
SGL_DEF_COLOUR(Bisque);
SGL_DEF_COLOUR(Black);
SGL_DEF_COLOUR(BlanchedAlmond);
SGL_DEF_COLOUR(Blue);
SGL_DEF_COLOUR(BlueViolet);
SGL_DEF_COLOUR(Brown);
SGL_DEF_COLOUR(BurlyWood);
SGL_DEF_COLOUR(CadetBlue);
SGL_DEF_COLOUR(Chartreuse);
SGL_DEF_COLOUR(Chocolate);
SGL_DEF_COLOUR(Coral);
SGL_DEF_COLOUR(CornflowerBlue);
SGL_DEF_COLOUR(Cornsilk);
SGL_DEF_COLOUR(Crimson);
SGL_DEF_COLOUR(Cyan);
SGL_DEF_COLOUR(DarkBlue);
SGL_DEF_COLOUR(DarkCyan);
SGL_DEF_COLOUR(DarkGoldenRod);
SGL_DEF_COLOUR(DarkGray);
SGL_DEF_COLOUR(DarkGreen);
SGL_DEF_COLOUR(DarkKhaki);
SGL_DEF_COLOUR(DarkMagenta);
SGL_DEF_COLOUR(DarkOliveGreen);
SGL_DEF_COLOUR(DarkOrange);
SGL_DEF_COLOUR(DarkOrchid);
SGL_DEF_COLOUR(DarkRed);
SGL_DEF_COLOUR(DarkSalmon);
SGL_DEF_COLOUR(DarkSeaGreen);
SGL_DEF_COLOUR(DarkSlateBlue);
SGL_DEF_COLOUR(DarkSlateGray);
SGL_DEF_COLOUR(DarkTurquoise);
SGL_DEF_COLOUR(DarkViolet);
SGL_DEF_COLOUR(DeepPink);
SGL_DEF_COLOUR(DeepSkyBlue);
SGL_DEF_COLOUR(DimGray);
SGL_DEF_COLOUR(DodgerBlue);
SGL_DEF_COLOUR(FireBrick);
SGL_DEF_COLOUR(FloralWhite);
SGL_DEF_COLOUR(ForestGreen);
SGL_DEF_COLOUR(Fuchsia);
SGL_DEF_COLOUR(Gainsboro);
SGL_DEF_COLOUR(GhostWhite);
SGL_DEF_COLOUR(Gold);
SGL_DEF_COLOUR(GoldenRod);
SGL_DEF_COLOUR(Gray);
SGL_DEF_COLOUR(Green);
SGL_DEF_COLOUR(GreenYellow);
SGL_DEF_COLOUR(HoneyDew);
SGL_DEF_COLOUR(HotPink);
SGL_DEF_COLOUR(IndianRed);
SGL_DEF_COLOUR(Indigo);
SGL_DEF_COLOUR(Ivory);
SGL_DEF_COLOUR(Khaki);
SGL_DEF_COLOUR(Lavender);
SGL_DEF_COLOUR(LavenderBlush);
SGL_DEF_COLOUR(LawnGreen);
SGL_DEF_COLOUR(LemonChiffon);
SGL_DEF_COLOUR(LightBlue);
SGL_DEF_COLOUR(LightCoral);
SGL_DEF_COLOUR(LightCyan);
SGL_DEF_COLOUR(LightGoldenRodYellow);
SGL_DEF_COLOUR(LightGray);
SGL_DEF_COLOUR(LightGreen);
SGL_DEF_COLOUR(LightPink);
SGL_DEF_COLOUR(LightSalmon);
SGL_DEF_COLOUR(LightSeaGreen);
SGL_DEF_COLOUR(LightSkyBlue);
SGL_DEF_COLOUR(LightSlateGray);
SGL_DEF_COLOUR(LightSteelBlue);
SGL_DEF_COLOUR(LightYellow);
SGL_DEF_COLOUR(Lime);
SGL_DEF_COLOUR(LimeGreen);
SGL_DEF_COLOUR(Linen);
SGL_DEF_COLOUR(Magenta);
SGL_DEF_COLOUR(Maroon);
SGL_DEF_COLOUR(MediumAquaMarine);
SGL_DEF_COLOUR(MediumBlue);
SGL_DEF_COLOUR(MediumOrchid);
SGL_DEF_COLOUR(MediumPurple);
SGL_DEF_COLOUR(MediumSeaGreen);
SGL_DEF_COLOUR(MediumSlateBlue);
SGL_DEF_COLOUR(MediumSpringGreen);
SGL_DEF_COLOUR(MediumTurquoise);
SGL_DEF_COLOUR(MediumVioletRed);
SGL_DEF_COLOUR(MidnightBlue);
SGL_DEF_COLOUR(MintCream);
SGL_DEF_COLOUR(MistyRose);
SGL_DEF_COLOUR(Moccasin);
SGL_DEF_COLOUR(NavajoWhite);
SGL_DEF_COLOUR(Navy);
SGL_DEF_COLOUR(NavyBlue);
SGL_DEF_COLOUR(OldLace);
SGL_DEF_COLOUR(Olive);
SGL_DEF_COLOUR(OliveDrab);
SGL_DEF_COLOUR(Orange);
SGL_DEF_COLOUR(OrangeRed);
SGL_DEF_COLOUR(Orchid);
SGL_DEF_COLOUR(PaleGoldenRod);
SGL_DEF_COLOUR(PaleGreen);
SGL_DEF_COLOUR(PaleTurquoise);
SGL_DEF_COLOUR(PaleVioletRed);
SGL_DEF_COLOUR(PapayaWhip);
SGL_DEF_COLOUR(PeachPuff);
SGL_DEF_COLOUR(Peru);
SGL_DEF_COLOUR(Pink);
SGL_DEF_COLOUR(Plum);
SGL_DEF_COLOUR(PowderBlue);
SGL_DEF_COLOUR(Purple);
SGL_DEF_COLOUR(RebeccaPurple);
SGL_DEF_COLOUR(Red);
SGL_DEF_COLOUR(RosyBrown);
SGL_DEF_COLOUR(RoyalBlue);
SGL_DEF_COLOUR(SaddleBrown);
SGL_DEF_COLOUR(Salmon);
SGL_DEF_COLOUR(SandyBrown);
SGL_DEF_COLOUR(SeaGreen);
SGL_DEF_COLOUR(SeaShell);
SGL_DEF_COLOUR(Sienna);
SGL_DEF_COLOUR(Silver);
SGL_DEF_COLOUR(SkyBlue);
SGL_DEF_COLOUR(SlateBlue);
SGL_DEF_COLOUR(SlateGray);
SGL_DEF_COLOUR(Snow);
SGL_DEF_COLOUR(SpringGreen);
SGL_DEF_COLOUR(SteelBlue);
SGL_DEF_COLOUR(Tan);
SGL_DEF_COLOUR(Teal);
SGL_DEF_COLOUR(Thistle);
SGL_DEF_COLOUR(Tomato);
SGL_DEF_COLOUR(Transparent);
SGL_DEF_COLOUR(Turquoise);
SGL_DEF_COLOUR(Violet);
SGL_DEF_COLOUR(Wheat);
SGL_DEF_COLOUR(White);
SGL_DEF_COLOUR(WhiteSmoke);
SGL_DEF_COLOUR(Yellow);
SGL_DEF_COLOUR(YellowGreen);

SGL_END

#undef SGL_DEF_COLOUR