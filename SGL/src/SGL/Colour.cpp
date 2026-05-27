#include "Colour.h"

sgl_Vec3 sgl_Colour_ToVec3(sgl_Colour c) {
	return { {{ c.r, c.g, c.b }} };
}

sgl_Vec4 sgl_Colour_ToVec4(sgl_Colour c) {
	return { {{ c.r, c.g, c.b, c.a }} };
}

sgl_PackedColour sgl_Colour_Pack(sgl_Colour colour)
{
	return
		((uint32)colour.r * 255) << 24 |
		((uint32)colour.g * 255) << 16 |
		((uint32)colour.b * 255) << 8 |
		((uint32)colour.a * 255);
}

sgl_Colour sgl_Colour_Unpack(sgl_PackedColour colour)
{
	int r = (colour >> 24) & 0xff;
	int g = (colour >> 16) & 0xff;
	int b = (colour >>  8) & 0xff;
	int a = colour & 0xff;
		
	return sgl_Colour_ByteA(r, g, b, a);
}

sgl_Colour sgl_Colour_Float(float r, float g, float b) {
	return sgl_Colour_FloatA(r, g, b, 1);
}

sgl_Colour sgl_Colour_FloatA(float r, float g, float b, float a) {
	return { r, g, b, a };
}

sgl_Colour sgl_Colour_Byte(uint8 r, uint8 g, uint8 b) {
	return sgl_Colour_ByteA(r, g, b, 255);
}

sgl_Colour sgl_Colour_ByteA(uint8 r, uint8 g, uint8 b, uint8 a) {
	return { r / 255.f, g / 255.f, b / 255.f, a / 255.f };
}

#define SGL_INIT_COLOUR(name, hex) const sgl_Colour sgl_Col_##name = sgl_Colour_Unpack(0x##hex##FF)

SGL_INIT_COLOUR(AliceBlue, F0F8FF);
SGL_INIT_COLOUR(AntiqueWhite, FAEBD7);
SGL_INIT_COLOUR(Aqua, 00FFFF);
SGL_INIT_COLOUR(Aquamarine, 7FFFD4);
SGL_INIT_COLOUR(Azure, F0FFFF);
SGL_INIT_COLOUR(Beige, F5F5DC);
SGL_INIT_COLOUR(Bisque, FFE4C4);
SGL_INIT_COLOUR(Black, 000000);
SGL_INIT_COLOUR(BlanchedAlmond, FFEBCD);
SGL_INIT_COLOUR(Blue, 0000FF);
SGL_INIT_COLOUR(BlueViolet, 8A2BE2);
SGL_INIT_COLOUR(Brown, A52A2A);
SGL_INIT_COLOUR(BurlyWood, DEB887);
SGL_INIT_COLOUR(CadetBlue, 5F9EA0);
SGL_INIT_COLOUR(Chartreuse, 7FFF00);
SGL_INIT_COLOUR(Chocolate, D2691E);
SGL_INIT_COLOUR(Coral, FF7F50);
SGL_INIT_COLOUR(CornflowerBlue, 6495ED);
SGL_INIT_COLOUR(Cornsilk, FFF8DC);
SGL_INIT_COLOUR(Crimson, DC143C);
SGL_INIT_COLOUR(Cyan, 00FFFF);
SGL_INIT_COLOUR(DarkBlue, 00008B);
SGL_INIT_COLOUR(DarkCyan, 008B8B);
SGL_INIT_COLOUR(DarkGoldenRod, B8860B);
SGL_INIT_COLOUR(DarkGray, A9A9A9);
SGL_INIT_COLOUR(DarkGreen, 006400);
SGL_INIT_COLOUR(DarkKhaki, BDB76B);
SGL_INIT_COLOUR(DarkMagenta, 8B008B);
SGL_INIT_COLOUR(DarkOliveGreen, 556B2F);
SGL_INIT_COLOUR(DarkOrange, FF8C00);
SGL_INIT_COLOUR(DarkOrchid, 9932CC);
SGL_INIT_COLOUR(DarkRed, 8B0000);
SGL_INIT_COLOUR(DarkSalmon, E9967A);
SGL_INIT_COLOUR(DarkSeaGreen, 8FBC8F);
SGL_INIT_COLOUR(DarkSlateBlue, 483D8B);
SGL_INIT_COLOUR(DarkSlateGray, 2F4F4F);
SGL_INIT_COLOUR(DarkTurquoise, 00CED1);
SGL_INIT_COLOUR(DarkViolet, 9400D3);
SGL_INIT_COLOUR(DeepPink, FF1493);
SGL_INIT_COLOUR(DeepSkyBlue, 00BFFF);
SGL_INIT_COLOUR(DimGray, 696969);
SGL_INIT_COLOUR(DodgerBlue, 1E90FF);
SGL_INIT_COLOUR(FireBrick, B22222);
SGL_INIT_COLOUR(FloralWhite, FFFAF0);
SGL_INIT_COLOUR(ForestGreen, 228B22);
SGL_INIT_COLOUR(Fuchsia, FF00FF);
SGL_INIT_COLOUR(Gainsboro, DCDCDC);
SGL_INIT_COLOUR(GhostWhite, F8F8FF);
SGL_INIT_COLOUR(Gold, FFD700);
SGL_INIT_COLOUR(GoldenRod, DAA520);
SGL_INIT_COLOUR(Gray, 808080);
SGL_INIT_COLOUR(Green, 008000);
SGL_INIT_COLOUR(GreenYellow, ADFF2F);
SGL_INIT_COLOUR(HoneyDew, F0FFF0);
SGL_INIT_COLOUR(HotPink, FF69B4);
SGL_INIT_COLOUR(IndianRed, CD5C5C);
SGL_INIT_COLOUR(Indigo, 4B0082);
SGL_INIT_COLOUR(Ivory, FFFFF0);
SGL_INIT_COLOUR(Khaki, F0E68C);
SGL_INIT_COLOUR(Lavender, E6E6FA);
SGL_INIT_COLOUR(LavenderBlush, FFF0F5);
SGL_INIT_COLOUR(LawnGreen, 7CFC00);
SGL_INIT_COLOUR(LemonChiffon, FFFACD);
SGL_INIT_COLOUR(LightBlue, ADD8E6);
SGL_INIT_COLOUR(LightCoral, F08080);
SGL_INIT_COLOUR(LightCyan, E0FFFF);
SGL_INIT_COLOUR(LightGoldenRodYellow, FAFAD2);
SGL_INIT_COLOUR(LightGray, D3D3D3);
SGL_INIT_COLOUR(LightGreen, 90EE90);
SGL_INIT_COLOUR(LightPink, FFB6C1);
SGL_INIT_COLOUR(LightSalmon, FFA07A);
SGL_INIT_COLOUR(LightSeaGreen, 20B2AA);
SGL_INIT_COLOUR(LightSkyBlue, 87CEFA);
SGL_INIT_COLOUR(LightSlateGray, 778899);
SGL_INIT_COLOUR(LightSteelBlue, B0C4DE);
SGL_INIT_COLOUR(LightYellow, FFFFE0);
SGL_INIT_COLOUR(Lime, 00FF00);
SGL_INIT_COLOUR(LimeGreen, 32CD32);
SGL_INIT_COLOUR(Linen, FAF0E6);
SGL_INIT_COLOUR(Magenta, FF00FF);
SGL_INIT_COLOUR(Maroon, 800000);
SGL_INIT_COLOUR(MediumAquaMarine, 66CDAA);
SGL_INIT_COLOUR(MediumBlue, 0000CD);
SGL_INIT_COLOUR(MediumOrchid, BA55D3);
SGL_INIT_COLOUR(MediumPurple, 9370DB);
SGL_INIT_COLOUR(MediumSeaGreen, 3CB371);
SGL_INIT_COLOUR(MediumSlateBlue, 7B68EE);
SGL_INIT_COLOUR(MediumSpringGreen, 00FA9A);
SGL_INIT_COLOUR(MediumTurquoise, 48D1CC);
SGL_INIT_COLOUR(MediumVioletRed, C71585);
SGL_INIT_COLOUR(MidnightBlue, 191970);
SGL_INIT_COLOUR(MintCream, F5FFFA);
SGL_INIT_COLOUR(MistyRose, FFE4E1);
SGL_INIT_COLOUR(Moccasin, FFE4B5);
SGL_INIT_COLOUR(NavajoWhite, FFDEAD);
SGL_INIT_COLOUR(Navy, 000080);
SGL_INIT_COLOUR(OldLace, FDF5E6);
SGL_INIT_COLOUR(Olive, 808000);
SGL_INIT_COLOUR(OliveDrab, 6B8E23);
SGL_INIT_COLOUR(Orange, FFA500);
SGL_INIT_COLOUR(OrangeRed, FF4500);
SGL_INIT_COLOUR(Orchid, DA70D6);
SGL_INIT_COLOUR(PaleGoldenRod, EEE8AA);
SGL_INIT_COLOUR(PaleGreen, 98FB98);
SGL_INIT_COLOUR(PaleTurquoise, AFEEEE);
SGL_INIT_COLOUR(PaleVioletRed, DB7093);
SGL_INIT_COLOUR(PapayaWhip, FFEFD5);
SGL_INIT_COLOUR(PeachPuff, FFDAB9);
SGL_INIT_COLOUR(Peru, CD853F);
SGL_INIT_COLOUR(Pink, FFC0CB);
SGL_INIT_COLOUR(Plum, DDA0DD);
SGL_INIT_COLOUR(PowderBlue, B0E0E6);
SGL_INIT_COLOUR(Purple, 800080);
SGL_INIT_COLOUR(RebeccaPurple, 663399);
SGL_INIT_COLOUR(Red, FF0000);
SGL_INIT_COLOUR(RosyBrown, BC8F8F);
SGL_INIT_COLOUR(RoyalBlue, 4169E1);
SGL_INIT_COLOUR(SaddleBrown, 8B4513);
SGL_INIT_COLOUR(Salmon, FA8072);
SGL_INIT_COLOUR(SandyBrown, F4A460);
SGL_INIT_COLOUR(SeaGreen, 2E8B57);
SGL_INIT_COLOUR(SeaShell, FFF5EE);
SGL_INIT_COLOUR(Sienna, A0522D);
SGL_INIT_COLOUR(Silver, C0C0C0);
SGL_INIT_COLOUR(SkyBlue, 87CEEB);
SGL_INIT_COLOUR(SlateBlue, 6A5ACD);
SGL_INIT_COLOUR(SlateGray, 708090);
SGL_INIT_COLOUR(Snow, FFFAFA);
SGL_INIT_COLOUR(SpringGreen, 00FF7F);
SGL_INIT_COLOUR(SteelBlue, 4682B4);
SGL_INIT_COLOUR(Tan, D2B48C);
SGL_INIT_COLOUR(Teal, 008080);
SGL_INIT_COLOUR(Thistle, D8BFD8);
SGL_INIT_COLOUR(Tomato, FF6347);
SGL_INIT_COLOUR(Transparent, 00000000);
SGL_INIT_COLOUR(Turquoise, 40E0D0);
SGL_INIT_COLOUR(Violet, EE82EE);
SGL_INIT_COLOUR(Wheat, F5DEB3);
SGL_INIT_COLOUR(White, FFFFFF);
SGL_INIT_COLOUR(WhiteSmoke, F5F5F5);
SGL_INIT_COLOUR(Yellow, FFFF00);
SGL_INIT_COLOUR(YellowGreen, 9ACD32);